# Technical evidence

This page records observable properties of the maintained project, measured locally on 2026-10-25. It does not claim performance improvements over the original submission: no controlled before/after performance benchmark was performed.

## Concepts demonstrated

- **C99 and data representation:** an 8 × 8 integer matrix and a small `Game` structure encode pieces and turns.
- **Algorithmic validation:** diagonal direction, ownership, occupancy, jump landing, capture, promotion, and winner detection are explicit functions.
- **Memory safety:** coordinates are validated before indexing; invalid moves leave the state unchanged; C tests run under AddressSanitizer and UndefinedBehaviorSanitizer.
- **Resource lifetime management:** the desktop creates Allegro displays, event queues, timers, bitmaps, samples, and streams, and destroys successfully created resources through a common cleanup path, including initialization failures.
- **Separation of concerns:** the rule engine has no graphics, audio, browser, or operating-system dependency. The desktop and web presentations call the same C rules.
- **Portability:** CMake/pkg-config handle the desktop build. Clang/LLD compile the engine into a standalone WebAssembly module; JavaScript handles browser interaction.
- **Regression testing and CI:** native tests, GUI interaction, Wasm invariants, complete browser matches, and deployment checks are repeatable.

## Memory: what is allocated, and where

The rule engine does **not** use `malloc`, `calloc`, `realloc`, or `free`. Its fixed-size state and bounded operations need no dynamically growing board or move list in C. This is a design choice suited to a fixed 8 × 8 game, not evidence of an advanced custom allocator.

| Measurement | Observed value | Scope |
| --- | --- | --- |
| `sizeof(Game)` via the Wasm bridge | **260 bytes** | 64 four-byte integers plus one four-byte turn value on wasm32 |
| WebAssembly file | **2,258 bytes** | Clang 18, `-O2`, stripped standalone module in the recorded local build; compiler versions can change the output size |
| WebAssembly linear memory | **128 KiB** | Fixed initial and maximum capacity, including a configured 16 KiB stack; this is capacity, not a measurement of live heap use |
| WebAssembly imports | **0** | No JavaScript runtime services or WASI imports are needed by the rules |

The desktop Allegro library allocates memory internally for graphics and audio. The browser also uses memory for JavaScript, the DOM, decoded images, audio, and its own runtime. **The numbers above do not measure total process memory**, and do not establish that the whole game is allocation-free. Images and sound files are much larger than the rules module.

A single move is validated with a bounded number of checks. Counting pieces and finding available moves scan a fixed board. In a generalized N × N version, those scans would be O(N²), with a fixed number of candidate directions per piece. No recursive search, minimax AI, or pathfinding is claimed.

## Recorded checks

| Check | Observed result |
| --- | --- |
| Native C rules | **5,446 checks passed**, including **3,200 square/direction cases** across all four piece types |
| Native memory instrumentation | Rule tests passed with AddressSanitizer and UndefinedBehaviorSanitizer |
| Wasm state and memory | Reset, invalid coordinates, capture, turn changes, state size, zero imports, and rejection of memory growth checked |
| Seeded Wasm simulations | 30 runs, 587 captures, and 121 promotions; 16 reached a winner, 14 hit the 1,500-turn cap |
| Browser interaction | Desktop and emulated touch layouts: moves, invalid moves, passing, capture, restart/cancel, a complete match, replay, opt-in audio, and failed-engine loading |
| Native platforms | Windows and Linux builds and rule tests; Linux desktop interaction |

The simulation cap reflects the absence of automatic draw detection. It is not reported as 30 completed games. Test counts are assertions and scenarios, **not a code-coverage percentage**. Sanitizers improve confidence for the paths exercised; they do not prove the absence of every possible defect.

## Reproduce

Native rule checks without Allegro:

```sh
cmake -S . -B build-rules -DBUILD_GAME=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build-rules --parallel
./build-rules/test-game
```

Browser engine build and measurements (Python 3, Clang, LLD, and Node.js):

```sh
python3 scripts/build_web.py
node tests/test_web_engine.mjs
```

The build writes `build-web/build-info.json`; the Node test prints the module size, state size, memory capacity, imports, and seeded simulation results. These outputs are also visible in the browser workflow logs.

End-to-end browser checks:

```sh
npm ci
npx playwright install chromium
npm run test:web
```

Playwright starts a temporary local server. Tests use desktop Chromium and Pixel 5 viewport/touch emulation, not a physical Pixel phone. See [development notes](DEVELOPMENT.md) for the native GUI test and sanitizer commands.

## Describing this work on a résumé

A fact-based description could be:

> Developed a space-themed two-player checkers game in C and Allegro for a UFSM programming course; later modernized its build and validation, preserved the submitted source, and deployed a browser edition that reuses the C rule engine through WebAssembly, with automated native and browser tests.

This description distinguishes the original coursework from the later modernization; the measurements and test results above support its technical claims.

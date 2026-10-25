# Development notes

## Build layout

`src/game.c` owns the 8 × 8 board and current turn. It has no Allegro dependency, which allows rule tests to run without a display or audio device. Piece values preserve the original representation: `0` empty, `1` blue, `2` pink, `3` blue king, `4` pink king.

`src/main.c` manages five screens: menu, rules, countdown, game, and victory. A 30 Hz Allegro timer drives rendering. The countdown uses elapsed time instead of blocking sleeps, so keyboard and window-close events continue to work. State is reset when a new game starts.

The renderer uses the original 640 × 640 artwork and 80 × 80 cells. Asset names remain in Portuguese to preserve the original files. CMake copies resources next to the executable; Allegro's resource path makes launching independent of the shell's working directory. Missing images produce an error with the filename. Missing audio is nonfatal.

## Changes from the original submission

The original code and Code::Blocks configuration are stored byte-for-byte in `legacy/`. The maintained version separates game rules from the interface and addresses these issues:

- Validate coordinates before indexing the board, including capture landing squares.
- Validate the entire move before changing the board or turn.
- Count kings as remaining pieces and preserve their type during captures.
- Handle promotion for both ordinary moves and captures.
- Reset the board, turn, winner, and selection when starting again.
- Render the initial menu and subsequent screens without waiting for mouse movement.
- Process window-close events on every screen and release resources on exit.
- Replace hardcoded `C:/Allegro` libraries and the Windows-only console clear with a CMake/pkg-config build.

The original code tried to perform some automatic double captures, but did not handle them consistently. The maintained version deliberately uses one capture per turn. It keeps the original forward-only regular pieces, short-range kings, optional captures, left/right mouse controls, and pass-on-same-square behavior. A blocked player now loses rather than leaving the match stuck. These are the rules documented in the README, not a claim of compliance with a tournament checkers ruleset.

## Run rule tests without Allegro

A C99 compiler and CMake are sufficient:

```sh
cmake -S . -B build-rules -DBUILD_GAME=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build-rules --parallel
ctest --test-dir build-rules --output-on-failure
```

The test checks remain active in Release builds. Coverage includes every playable square and diagonal direction for all four piece types, invalid coordinates, blocked captures, backward moves, promotion, king captures and counts, passing, wins, and reset.

## Memory checks on Linux

With GCC or Clang:

```sh
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_FLAGS="-Werror -fsanitize=address,undefined -fno-omit-frame-pointer"
cmake --build build-sanitize --parallel
ctest --test-dir build-sanitize --output-on-failure
```

## Desktop smoke test on Linux

Install the game dependencies from the README, plus:

```sh
sudo apt-get install python3 xvfb xdotool
```

Then run:

```sh
xvfb-run -a python3 tests/gui_smoke.py build/damas-espaciais
```

The test launches the actual executable from a temporary directory with audio disabled. It checks navigation, the countdown, piece selection, moves, captures, invalid moves, passing, turn counts, restarting, and exiting. It uses Xvfb to avoid interacting with the user's desktop.

For a sanitizer build, add `ASAN_OPTIONS=detect_leaks=0` before `xvfb-run`. This avoids reports from process-global allocations retained by the graphics stack while leaving invalid-memory-access and undefined-behavior checks enabled. Rule tests use the normal sanitizer settings.

To exercise sound loading and playback calls as well, append `--with-audio`. With an OpenAL backend, `ALSOFT_DRIVERS=null` selects a silent output for environments without speakers. This checks the audio code path, not audible output quality.

To capture the opening board, install Pillow (`python3-pil`) and set `DAMAS_SCREENSHOT` to an absolute PNG output path when running the smoke test. The README screenshot was captured from the maintained game.

## Continuous integration

`.github/workflows/build.yml` runs on pushes and pull requests:

- Ubuntu 24.04: compile with warnings treated as errors, AddressSanitizer and UndefinedBehaviorSanitizer; run rule and desktop tests.
- Windows: compile with MSYS2 UCRT64 and warnings treated as errors; run rule tests.

The Windows job does not automate the GUI. Audio output still needs a manual check on a machine with speakers or headphones. Tests do not establish support for other operating systems.

## Manual release checks

1. Open the menu, rules, and game; confirm the countdown remains responsive.
2. Check soundtrack playback and selection/invalid-move effects.
3. Play both colors, including a capture, promotion, and a king moving backward.
4. Finish a game, check its winner screen and music, then try both replay and exit.
5. Confirm a new game begins with blue and 12 pieces per player.

Keep binaries, object files, editor state, desktop metadata, and local build folders out of commits. Keep the original source in `legacy/` unchanged when modifying the maintained game.

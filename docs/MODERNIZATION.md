# Original submission and modernization

## What this repository preserves

**`legacy/main.c` and `legacy/Damas Espaciais.cbp` are the original files submitted for the UFSM Algorithms and Programming course**, taken unchanged from `Damas Espaciais.zip`. They are a historical record of the student's work at the time, including its limitations. The original project used C, Allegro 5, and Code::Blocks 17.12 on Windows.

The maintained desktop code is in `src/`. The browser interface is in `web/`. These are subsequent updates, not the original submitted code. The purpose is to keep the project accessible on current systems, correct errors that affected play, and let visitors try it without installing the desktop toolchain.

The overall project remains a space-themed, local two-player checkers game. Its 8 × 8 matrix, blue and pink fleets, original artwork, sounds, and educational purpose are retained. The update includes a substantial organization of the implementation and a small number of deliberate rule clarifications; it would be inaccurate to describe every change as only a compatibility fix.

## Before and after

| Area | Original submission | Maintained editions |
| --- | --- | --- |
| Build | Code::Blocks project with absolute `C:/Allegro` paths and Allegro 5.0.10 libraries | CMake and pkg-config with current Allegro 5.2 packages; tested Windows and Linux builds |
| Access | Download and configure the desktop development environment | Desktop remains available; GitHub Pages adds a browser edition with no local installation |
| Organization | Graphics, input, globals, and game rules in a single C source file | Shared `game.c` engine, Allegro desktop UI, and a separate browser UI |
| Board boundaries | Some neighboring squares were accessed before checking coordinates | Coordinates and capture landings are checked before board access |
| Move validation | Some paths could change turn or board state before a failed capture was resolved | Invalid moves preserve the board and the turn |
| Piece counting | Only regular pieces were included in the totals | Regular pieces and kings both count |
| Promotion and captures | Some capture paths lost or incorrectly handled the piece type | Both colors promote on reaching the far edge; captured kings and moving kings are handled consistently |
| Restart | Turn, counters, and selection could retain stale state | A new game resets the board and begins with 12 pieces per player, blue first |
| Screens and exit | Drawing depended on mouse movement; countdown used blocking sleeps; some close paths were incomplete | Desktop rendering is timer-driven, events remain responsive, and resources are released on exit |
| Resources | Relative paths assumed a particular working directory; audio failure stopped startup | Desktop assets resolve beside the executable; unavailable audio is nonfatal; browser sound is opt-in |
| Validation | No automated suite in the supplied ZIP | C tests, sanitizer checks, Wasm tests, and desktop/mobile browser tests in CI |
| Documentation | Portuguese README with an external dependency ZIP | English build instructions, actual test evidence, provenance, and a live playable demo |

## Gameplay continuity and deliberate differences

Both maintained editions keep blue moving first, forward-only regular pieces, short-range kings, optional captures, promotion at the opposite edge, and the original ability to pass. They remain a custom educational variant rather than tournament checkers.

Two differences from the old implementation are explicit:

- The original attempted automatic double captures in some directions, inconsistently. The maintained rules allow **one capture per turn** and do not chain jumps.
- A player with no legal moves now loses, preventing a blocked match from waiting indefinitely.

The browser uses **the same `src/game.c`**, compiled to WebAssembly. JavaScript handles the interface; it does not reimplement the rules. It supports selecting and moving with clicks, taps, or keyboard input. The desktop's original right-click controls remain available. In the browser, a dedicated Pass button avoids accidentally passing by tapping a selected piece twice. The browser opens directly on the board instead of showing the original menu and countdown.

No AI opponent, online matchmaking, save system, or automatic draw detection has been added. The original images are unchanged; their embedded Portuguese text remains part of the desktop artwork. The browser's surrounding interface and documentation are in English.

## Compatibility, with a concrete scope

The aim is easier access on current computers, not a guarantee for every device:

- **Desktop:** Windows with MSYS2 UCRT64 and Linux with Allegro 5.2. Both are built and their game rules tested in GitHub Actions; the Linux GUI is also exercised automatically.
- **Browser:** a current browser with JavaScript and WebAssembly. Chromium desktop and an emulated mobile viewport with touch input are tested. This is not a claim that every browser, physical phone, or operating system has been manually tested.
- **Audio:** optional. Browser policies require a user action before playback. Sound files load only after sound is enabled.
- **Offline access:** there is no service worker or offline-installation feature; the site needs a connection to load its files.

## Verify the preserved submission

From the repository root:

```sh
sha256sum -c legacy/SHA256SUMS
```

The SHA-256 values were calculated after comparing both preserved files byte-for-byte against the supplied ZIP. `.gitattributes` disables text conversion for these files, retaining the original encoding and line endings even on Windows.

The original `imagens/` and `sons/` resources are also preserved. Prebuilt executables, object files, editor session files, and desktop metadata from the ZIP were not imported. Its `License.txt.txt` is preserved as the root `LICENSE`.

See [technical evidence](TECHNICAL_EVIDENCE.md) for measurements and validation commands, and [development notes](DEVELOPMENT.md) for the build and test workflow.

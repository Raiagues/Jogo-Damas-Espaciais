# Original course submission

`main.c` and `Damas Espaciais.cbp` are preserved unchanged from the supplied `Damas Espaciais.zip` archive. The original program was developed with Code::Blocks 17.12 and a Windows Allegro 5.0.10 installation under `C:/Allegro`.

These are the files submitted for the course and document the original academic work. Their hashes are recorded in `SHA256SUMS` (verify from the repository root with `sha256sum -c legacy/SHA256SUMS`). They are not the default build and still contain the original movement, piece-counting, and boundary-access defects. The Code::Blocks file also retains its original absolute library paths and `Pong` output names.

Use the repository's root `CMakeLists.txt` to build the maintained game in `src/`. It uses the original artwork and audio in `imagens/` and `sons/`. See the [main README](../README.md) for download, build, and play instructions, and [development notes](../docs/DEVELOPMENT.md) for the changes.

The ZIP's prebuilt Windows executables, object files, IDE session files, `Thumbs.db`, and `desktop.ini` are intentionally excluded. The supplied `License.txt.txt` is preserved as the root `LICENSE` file.

See the [before-and-after comparison](../docs/MODERNIZATION.md) for the distinction between the original submission and the later desktop/browser modernization.

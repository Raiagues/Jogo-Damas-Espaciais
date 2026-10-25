#!/usr/bin/env python3
"""Build the static Pages site and the shared C rules as standalone WebAssembly."""
from pathlib import Path
import argparse
import json
import shutil
import subprocess

root = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument('--output', type=Path, default=root / 'build-web')
args = parser.parse_args()
output = args.output.resolve()
output.mkdir(parents=True, exist_ok=True)
exports = ['reset', 'turn', 'piece', 'move', 'can_move', 'count', 'winner', 'state_bytes']
subprocess.run([
    'clang', '--target=wasm32-unknown-unknown', '-std=c99', '-O2', '-nostdlib',
    '-fno-builtin', '-Wall', '-Wextra', '-Wpedantic', '-Werror',
    '-Wl,--no-entry', '-Wl,--export-memory', '-Wl,--initial-memory=131072',
    '-Wl,--max-memory=131072', '-Wl,-z,stack-size=16384', '-Wl,--strip-all',
    *[f'-Wl,--export=web_{name}' for name in exports],
    str(root / 'src/game.c'), str(root / 'src/web.c'), '-o', str(output / 'game.wasm'),
], check=True)
for file in (root / 'web').iterdir():
    if file.is_file():
        shutil.copy2(file, output / file.name)
for folder in ['imagens', 'sons']:
    shutil.copytree(root / folder, output / folder, dirs_exist_ok=True)
shutil.copy2(root / 'LICENSE', output / 'LICENSE')
(output / '.nojekyll').touch()
metrics = {'wasm_bytes': (output / 'game.wasm').stat().st_size,
           'linear_memory_bytes': 131072,
           'note': 'Wasm module and linear memory only; excludes browser, DOM, artwork, audio, and desktop Allegro.'}
(output / 'build-info.json').write_text(json.dumps(metrics, indent=2) + '\n')
print(json.dumps(metrics, indent=2))
print(f'Site ready in {output}')

import assert from 'node:assert/strict';
import { readFile } from 'node:fs/promises';

const bytes = await readFile(new URL('../build-web/game.wasm', import.meta.url));
const module = await WebAssembly.compile(bytes);
assert.equal(WebAssembly.Module.imports(module).length, 0, 'The engine must be standalone');
const { exports: game } = await WebAssembly.instantiate(module);
const snapshot = () => Array.from({ length: 64 }, (_, i) => game.web_piece(i >> 3, i % 8));
const owner = (piece) => piece === 1 || piece === 3 ? 1 : piece === 2 || piece === 4 ? 2 : 0;
game.web_reset();
assert.equal(game.web_state_bytes(), 260);
assert.equal(game.memory.buffer.byteLength, 131072);
assert.throws(() => game.memory.grow(1), RangeError);
assert.equal(game.web_turn(), 1);
assert.equal(game.web_count(1), 12);
assert.equal(game.web_count(2), 12);
assert.equal(game.web_winner(), 0);
for (const [r, c] of [[-1, 0], [0, -1], [8, 0], [0, 8], [2147483647, 0]]) {
  const before = snapshot();
  assert.equal(game.web_move(r, c, 3, 0), 0);
  assert.equal(game.web_move(2, 1, r, c), 0);
  assert.equal(game.web_piece(r, c), -1);
  assert.deepEqual(snapshot(), before);
  assert.equal(game.web_turn(), 1);
}
assert.equal(game.web_move(2, 1, 3, 0), 1);
assert.equal(game.web_move(5, 0, 4, 1), 1);
assert.equal(game.web_move(3, 0, 4, 1), 0); // Occupied landing square.
assert.equal(game.web_move(3, 0, 3, 0), 1); // Pass.
assert.equal(game.web_move(5, 2, 4, 3), 1);
assert.equal(game.web_move(3, 0, 4, 1), 1);
assert.equal(game.web_piece(5, 2), 1);
assert.equal(game.web_count(2), 11);

let state = 20261025;
const random = (count) => { state = (Math.imul(state, 1664525) + 1013904223) >>> 0; return state % count; };
let completed = 0;
let promoted = 0;
let captures = 0;
for (let match = 0; match < 30; ++match) {
  game.web_reset();
  for (let ply = 0; ply < 1500 && !game.web_winner(); ++ply) {
    const options = [], jumps = [];
    const turn = game.web_turn();
    for (let r = 0; r < 8; ++r) for (let c = 0; c < 8; ++c) {
      if (owner(game.web_piece(r, c)) !== turn) continue;
      for (let dr = -2; dr <= 2; ++dr) for (let dc = -2; dc <= 2; ++dc) {
        if (game.web_piece(r + dr, c + dc) !== 0) continue;
        if (game.web_can_move(r, c, r + dr, c + dc)) {
          const move = [r, c, r + dr, c + dc];
          options.push(move);
          if (Math.abs(dr) === 2) jumps.push(move);
        }
      }
    }
    assert.ok(options.length, 'An unfinished position has a legal move');
    const candidates = jumps.length ? jumps : options;
    const move = candidates[random(candidates.length)];
    const before = [game.web_count(1), game.web_count(2)];
    const piece = game.web_piece(move[0], move[1]);
    const capture = Math.abs(move[2] - move[0]) === 2;
    assert.equal(game.web_move(...move), 1);
    assert.equal(game.web_turn(), 3 - turn);
    assert.equal(game.web_count(turn), before[turn - 1]);
    assert.equal(game.web_count(3 - turn), before[2 - turn] - Number(capture));
    if (capture) ++captures;
    if (piece <= 2 && game.web_piece(move[2], move[3]) > 2) ++promoted;
    for (const [index, value] of snapshot().entries()) {
      assert.ok(value >= 0 && value <= 4);
      if (value) assert.equal(((index >> 3) + index % 8) % 2, 1);
    }
  }
  if (game.web_winner()) {
    ++completed;
    const before = snapshot();
    assert.equal(game.web_move(0, 1, 1, 2), 0, 'A finished match cannot be changed');
    assert.deepEqual(snapshot(), before);
  }
  assert.equal(game.memory.buffer.byteLength, 131072);
}
assert.ok(completed > 0 && promoted > 0 && captures > 0);
console.log(JSON.stringify({ wasm_bytes: bytes.length, state_bytes: game.web_state_bytes(),
  linear_memory_bytes: game.memory.buffer.byteLength, imports: 0, simulated_matches: 30,
  completed, promotions: promoted, captures }, null, 2));

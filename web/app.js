const $ = (id) => document.getElementById(id);
const names = ['empty', 'Blue piece', 'Pink piece', 'Blue king', 'Pink king'];
const images = ['', 'peca_azul_normal', 'peca_rosa_normal', 'peca_azul_dama', 'peca_rosa_dama'];
const playerName = (player) => player === 1 ? 'Blue' : 'Pink';
const owner = (piece) => piece === 1 || piece === 3 ? 1 : piece === 2 || piece === 4 ? 2 : 0;
let engine;
let selected = null;
let moves = 0;
let soundEnabled = false;
const cells = [];
const background = new Audio('sons/trilha_sonora_1.wav');
background.preload = 'none';
background.loop = true;
background.volume = 0.22;
const effects = Object.fromEntries(['som_selecionar', 'som_fail', 'som_campeao'].map((name) => {
  const audio = new Audio(`sons/${name}.wav`);
  audio.preload = 'none';
  audio.volume = 0.4;
  return [name, audio];
}));

function announce(message) { $('status').textContent = message; }
function playEffect(name) {
  if (!soundEnabled) return;
  const audio = effects[name];
  audio.currentTime = 0;
  audio.play().catch(() => {});
}
function soundOff() {
  soundEnabled = false;
  background.pause();
  for (const audio of Object.values(effects)) audio.pause();
  $('sound').setAttribute('aria-pressed', 'false');
  $('sound').textContent = 'Sound: off';
}

function render() {
  const turn = engine.web_turn();
  const winner = engine.web_winner();
  $('turn').textContent = winner ? `${playerName(winner)} wins!` : `${playerName(turn)}'s turn`;
  $('board-mode').textContent = winner ? 'MISSION COMPLETE' : 'LOCAL TWO-PLAYER';
  $('new-game').textContent = winner ? 'Play again ↗' : 'Start a new game ↗';
  for (const player of [1, 2]) {
    const color = player === 1 ? 'blue' : 'pink';
    $(`${color}-count`).textContent = engine.web_count(player);
    $(`${color}-fleet`).classList.toggle('active', winner ? winner === player : turn === player);
  }
  $('pass').disabled = !selected || Boolean(winner);
  cells.forEach((cell, index) => {
    const row = Math.floor(index / 8);
    const col = index % 8;
    const piece = engine.web_piece(row, col);
    const isSelected = selected?.[0] === row && selected?.[1] === col;
    const legal = !winner && selected && engine.web_can_move(...selected, row, col);
    cell.classList.toggle('selected', isSelected);
    cell.classList.toggle('legal', Boolean(legal));
    cell.classList.toggle('occupied', Boolean(piece));
    cell.setAttribute('aria-selected', String(isSelected));
    cell.setAttribute('aria-disabled', String(Boolean(winner) || (row + col) % 2 === 0));
    cell.setAttribute('aria-label', `${String.fromCharCode(65 + col)}${row + 1}, ${names[piece]}${isSelected ? ', selected' : ''}${legal ? ', legal destination' : ''}`);
    if (cell.dataset.piece !== String(piece)) {
      cell.replaceChildren();
      if (piece) {
        const image = document.createElement('img');
        image.src = `imagens/${images[piece]}.png`;
        image.alt = '';
        image.draggable = false;
        cell.append(image);
      }
      cell.dataset.piece = piece;
    }
  });
}

function reset() {
  engine.web_reset();
  selected = null;
  moves = 0;
  effects.som_campeao.pause();
  effects.som_campeao.currentTime = 0;
  if (soundEnabled) background.play().catch(() => {});
  announce('Blue launches first. Select a blue piece, then choose a highlighted destination.');
  render();
}

function move(row, col) {
  if (!engine.web_move(...selected, row, col)) {
    playEffect('som_fail');
    announce('That move is not available. Choose a highlighted square, or pass your turn.');
    return;
  }
  selected = null;
  ++moves;
  const winner = engine.web_winner();
  if (winner) {
    background.pause();
    playEffect('som_campeao');
    announce(`${playerName(winner)} wins. The other fleet has no pieces or legal moves left. Play again for a fresh board.`);
  } else {
    playEffect('som_selecionar');
    announce(`${playerName(engine.web_turn())}, select your next piece.`);
  }
  render();
}

function choose(index, rightClick = false) {
  if (!engine || engine.web_winner()) return;
  const row = Math.floor(index / 8);
  const col = index % 8;
  if (!selected) {
    if (owner(engine.web_piece(row, col)) !== engine.web_turn()) {
      announce(`Select a ${playerName(engine.web_turn()).toLowerCase()} piece to begin your turn.`);
      return;
    }
    selected = [row, col];
    playEffect('som_selecionar');
    announce('Piece selected. Choose a highlighted destination, or use Pass turn.');
    render();
  } else if (selected[0] === row && selected[1] === col && !rightClick) {
    announce('This piece is selected. Choose a highlighted destination, or use Pass turn.');
  } else {
    move(row, col);
  }
}

function makeBoard() {
  let rowElement;
  for (let index = 0; index < 64; ++index) {
    if (index % 8 === 0) {
      rowElement = document.createElement('div');
      rowElement.className = 'board-row';
      rowElement.setAttribute('role', 'row');
      $('board').append(rowElement);
    }
    const cell = document.createElement('button');
    cell.type = 'button';
    cell.className = 'cell';
    cell.setAttribute('role', 'gridcell');
    cell.setAttribute('aria-rowindex', Math.floor(index / 8) + 1);
    cell.setAttribute('aria-colindex', index % 8 + 1);
    cell.tabIndex = index === 17 ? 0 : -1;
    cell.addEventListener('click', () => choose(index));
    cell.addEventListener('contextmenu', (event) => { event.preventDefault(); choose(index, true); });
    cell.addEventListener('focus', () => cells.forEach((other) => { other.tabIndex = other === cell ? 0 : -1; }));
    cell.addEventListener('keydown', (event) => {
      const row = Math.floor(index / 8);
      const col = index % 8;
      let next = index;
      if (event.key === 'ArrowRight' && col < 7) ++next;
      else if (event.key === 'ArrowLeft' && col > 0) --next;
      else if (event.key === 'ArrowDown' && row < 7) next += 8;
      else if (event.key === 'ArrowUp' && row > 0) next -= 8;
      else if (!event.key.startsWith('Arrow')) return;
      event.preventDefault();
      cells[next].focus();
    });
    cells.push(cell);
    rowElement.append(cell);
  }
}

$('new-game').addEventListener('click', () => {
  if (moves && !engine.web_winner()) $('restart-dialog').showModal();
  else reset();
});
$('restart-dialog').addEventListener('close', () => {
  if ($('restart-dialog').returnValue === 'restart') reset();
});
$('pass').addEventListener('click', () => { if (selected) move(...selected); });
$('sound').addEventListener('click', async () => {
  if (soundEnabled) { soundOff(); return; }
  soundEnabled = true;
  $('sound').setAttribute('aria-pressed', 'true');
  $('sound').textContent = 'Sound: on';
  try {
    if (engine.web_winner()) await effects.som_campeao.play();
    else await background.play();
  } catch {
    soundOff();
    announce('Audio could not start. You can keep playing with sound off.');
  }
});
document.addEventListener('visibilitychange', () => {
  if (document.hidden) {
    background.pause();
    for (const audio of Object.values(effects)) audio.pause();
  } else if (soundEnabled && engine && !engine.web_winner()) background.play().catch(() => {});
});

try {
  const response = await fetch('game.wasm');
  if (!response.ok) throw new Error(`Game download failed (${response.status})`);
  const { instance } = await WebAssembly.instantiate(await response.arrayBuffer());
  engine = instance.exports;
  makeBoard();
  $('board').setAttribute('aria-busy', 'false');
  $('new-game').disabled = $('sound').disabled = false;
  reset();
} catch (error) {
  $('turn').textContent = 'The game could not load';
  announce('Please reload with an internet connection and a browser that supports WebAssembly. The desktop download is available through Source code.');
  console.error(error);
}

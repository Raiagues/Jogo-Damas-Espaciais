import { test, expect } from '@playwright/test';
import { readFile } from 'node:fs/promises';

const cell = (page, index) => page.locator('.cell').nth(index);
const activate = async (page, index, touch = false) => touch ? cell(page, index).tap() : cell(page, index).click();

test('play, capture, pass, restart, keyboard, and mobile layout', async ({ page, isMobile }) => {
  const errors = [];
  const audioRequests = [];
  page.on('pageerror', (error) => errors.push(error.message));
  page.on('request', (request) => { if (request.url().includes('/sons/')) audioRequests.push(request.url()); });
  await page.goto('./');
  await expect(page.locator('#turn')).toHaveText("Blue's turn");
  await expect(page.locator('.cell')).toHaveCount(64);
  await expect(page.locator('.cell img')).toHaveCount(24);
  await expect(page.locator('#pass')).toBeDisabled();
  expect(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth)).toBe(true);
  await activate(page, 17, isMobile);
  await expect(cell(page, 17)).toHaveAttribute('aria-selected', 'true');
  await activate(page, 24, isMobile);
  await expect(page.locator('#turn')).toHaveText("Pink's turn");
  await activate(page, 40, isMobile);
  await activate(page, 33, isMobile);
  await activate(page, 24, isMobile);
  await activate(page, 26, isMobile);
  await expect(page.locator('#status')).toContainText('not available');
  await expect(page.locator('#turn')).toHaveText("Blue's turn");
  await page.locator('#pass').click();
  await activate(page, 42, isMobile);
  await activate(page, 35, isMobile);
  await activate(page, 24, isMobile);
  await activate(page, 33, isMobile);
  await expect(page.locator('#pink-count')).toHaveText('11');
  await expect(cell(page, 42)).toHaveAttribute('data-piece', '1');
  await page.locator('#new-game').click();
  await expect(page.locator('dialog')).toBeVisible();
  await page.getByRole('button', { name: 'Keep playing' }).click();
  await expect(page.locator('#pink-count')).toHaveText('11');
  await page.locator('#new-game').click();
  await page.getByRole('button', { name: 'New game', exact: true }).click();
  await expect(page.locator('#turn')).toHaveText("Blue's turn");
  await expect(page.locator('#pink-count')).toHaveText('12');
  await expect(page.locator('.cell.selected')).toHaveCount(0);
  if (!isMobile) {
    await cell(page, 17).focus();
    await page.keyboard.press('Enter');
    await page.keyboard.press('ArrowDown');
    await page.keyboard.press('ArrowLeft');
    await page.keyboard.press('Space');
    await expect(page.locator('#turn')).toHaveText("Pink's turn");
  }
  expect(audioRequests).toEqual([]); // Original audio is opt-in, not part of initial loading.
  expect(errors).toEqual([]);
});

test('complete a match through the UI and replay', async ({ page, isMobile }) => {
  const { instance } = await WebAssembly.instantiate(await readFile('build-web/game.wasm'));
  const game = instance.exports;
  game.web_reset();
  let state = 7;
  await page.goto('./');
  await expect(page.locator('#turn')).toHaveText("Blue's turn");
  for (let ply = 0; ply < 500 && !game.web_winner(); ++ply) {
    const moves = [], jumps = [];
    for (let r = 0; r < 8; ++r) for (let c = 0; c < 8; ++c) {
      for (let dr = -2; dr <= 2; ++dr) for (let dc = -2; dc <= 2; ++dc) {
        if (game.web_piece(r + dr, c + dc) !== 0) continue;
        if (game.web_can_move(r, c, r + dr, c + dc)) {
          const move = [r, c, r + dr, c + dc];
          moves.push(move);
          if (Math.abs(dr) === 2) jumps.push(move);
        }
      }
    }
    const candidates = jumps.length ? jumps : moves;
    state = (Math.imul(state, 1664525) + 1013904223) >>> 0;
    const move = candidates[state % candidates.length];
    expect(game.web_move(...move)).toBe(1);
    await activate(page, move[0] * 8 + move[1], isMobile);
    await activate(page, move[2] * 8 + move[3], isMobile);
    await expect(page.locator('#blue-count')).toHaveText(String(game.web_count(1)));
    await expect(page.locator('#pink-count')).toHaveText(String(game.web_count(2)));
  }
  expect(game.web_winner()).toBeGreaterThan(0);
  await expect(page.locator('#turn')).toContainText('wins!');
  await expect(page.locator('#pass')).toBeDisabled();
  await page.getByRole('button', { name: 'Play again' }).click();
  await expect(page.locator('#turn')).toHaveText("Blue's turn");
  await expect(page.locator('.cell img')).toHaveCount(24);
});

test('show a useful error when the engine cannot load', async ({ page }) => {
  await page.route('**/game.wasm', (route) => route.fulfill({ status: 503, body: 'Unavailable' }));
  await page.goto('./');
  await expect(page.locator('#turn')).toHaveText('The game could not load');
  await expect(page.locator('#status')).toContainText('desktop download');
  await expect(page.locator('#new-game')).toBeDisabled();
});

test('sound loads only after opting in and can be turned off', async ({ page }) => {
  await page.goto('./');
  await expect(page.locator('#turn')).toHaveText("Blue's turn");
  const loaded = page.waitForResponse((response) => response.url().endsWith('/sons/trilha_sonora_1.wav') && response.ok());
  await page.locator('#sound').click();
  await loaded;
  await expect(page.locator('#sound')).toHaveAttribute('aria-pressed', 'true');
  await page.locator('#sound').click();
  await expect(page.locator('#sound')).toHaveAttribute('aria-pressed', 'false');
});

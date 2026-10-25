"""Exercise the desktop UI on Linux: xvfb-run -a python3 tests/gui_smoke.py build/damas-espaciais."""

import os
from pathlib import Path
import subprocess
import sys
import tempfile
import time


def xdo(*args):
    return subprocess.check_output(["xdotool", *map(str, args)], text=True).strip()


def wait_for(predicate, description, timeout=15):
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if process.poll() is not None:
            raise RuntimeError(f"Game exited while waiting for {description}")
        if predicate():
            return
        time.sleep(0.1)
    raise RuntimeError(f"Timed out waiting for {description}")


def title_contains(text):
    wait_for(lambda: text in xdo("getwindowname", window), text)


def click(x, y, button=1):
    xdo("mousemove", "--sync", "--window", window, x, y)
    # X11 numbers right-click as 3; Allegro exposes it as button 2.
    xdo("click", 3 if button == 2 else button)
    time.sleep(0.15)


executable = Path(sys.argv[1]).resolve()
with tempfile.TemporaryDirectory() as temporary, tempfile.TemporaryFile(mode="w+") as log:
    # Launch from an unrelated directory to catch broken resource lookup.
    audio_args = [] if "--with-audio" in sys.argv[2:] else ["--no-audio"]
    process = subprocess.Popen([str(executable), *audio_args], cwd=temporary,
                               stdout=log, stderr=log)
    try:
        windows = []

        def find_window():
            result = subprocess.run(["xdotool", "search", "--pid", str(process.pid),
                                     "--name", "Damas Espaciais"],
                                    capture_output=True, text=True)
            windows[:] = result.stdout.splitlines()
            return bool(windows)

        wait_for(find_window, "game window")
        window = windows[0]
        xdo("windowfocus", "--sync", window)
        title_contains("Menu")
        click(320, 440)
        title_contains("Rules")
        click(320, 590)
        title_contains("Menu")
        click(320, 350)
        title_contains("Countdown")
        title_contains("Blue's turn | Blue: 12 | Pink: 12")
        screenshot = os.environ.get("DAMAS_SCREENSHOT")
        if screenshot:
            from PIL import ImageGrab
            time.sleep(0.2)
            geometry = dict(line.split("=", 1) for line in
                            xdo("getwindowgeometry", "--shell", window).splitlines())
            x, y = int(geometry["X"]), int(geometry["Y"])
            ImageGrab.grab(bbox=(x, y, x + 640, y + 640)).save(screenshot)
        click(120, 200)  # Blue (2, 1).
        title_contains("Right-click to move")
        click(40, 280, 2)  # To (3, 0).
        title_contains("Pink's turn")
        click(40, 440)  # Pink (5, 0).
        click(120, 360, 2)  # To (4, 1).
        title_contains("Blue's turn")
        click(40, 280)
        click(40, 360, 2)  # Reject a non-diagonal move.
        title_contains("Invalid move")
        click(120, 360, 2)  # Reject capture with occupied landing (5, 2).
        title_contains("Invalid move")
        click(40, 280, 2)  # Pass via original control.
        title_contains("Pink's turn | Blue: 12 | Pink: 12")
        click(200, 440)  # Move the pink piece blocking the capture landing.
        click(280, 360, 2)
        title_contains("Blue's turn")
        click(40, 280)
        click(120, 360, 2)
        title_contains("Pink's turn | Blue: 12 | Pink: 11")
        xdo("key", "Escape")
        title_contains("Menu")
        click(320, 350)
        title_contains("Blue's turn | Blue: 12 | Pink: 12")
        # A fresh selection confirms selection state was reset too.
        click(280, 200)
        title_contains("Right-click to move")
        click(200, 280, 2)
        title_contains("Pink's turn")
        xdo("key", "Escape")
        title_contains("Menu")
        click(320, 350)
        title_contains("Countdown")
        xdo("key", "Escape")  # Countdown remains responsive.
        title_contains("Menu")
        xdo("key", "Escape")
        if process.wait(timeout=5) != 0:
            raise RuntimeError("Game did not exit cleanly")
        print("GUI smoke test passed: menus, moves, captures, invalid moves, pass, restart, countdown, exit.")
    finally:
        if process.poll() is None:
            process.terminate()
            process.wait(timeout=5)
        log.seek(0)
        output = log.read()
        if output:
            print(output, file=sys.stderr)

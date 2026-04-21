# 💣 Minesweeper Bullet Hell

A twist on classic Minesweeper: dodge a barrage of bullets while trying to clear the board.

Built with **C++** and **SFML**.

https://github.com/user-attachments/assets/0048f4b1-d14c-48c6-a8c8-69e3eea25e22

## Features

- **Classic Minesweeper:** left-click to reveal, right-click to flag, with mine counter, timer, and a leaderboard
- **Bullet Hell Mode:** toggleable overlay that fires projectiles at your cursor while you play
  - 5 attack patterns: carpet rain, random scatter, cursor-seeking, gapped carpets, and multi-stream spinners
  - 4 difficulty levels (Easy to Impossible) controlling bullet speed and fire rate
  - **God Mode:** survive bullet hits and track your death count instead of instantly losing
- **Sound effects:** distinct audio cues for each attack pattern and death events
- **Debug mode:** peek at mine locations mid-game

## Controls

| Input | Action |
|---|---|
| Left-click tile | Reveal tile |
| Right-click tile | Toggle flag |
| Smiley face button | Restart game |
| Pause button | Pause / resume (classic mode only) |
| Leaderboard button | View top 5 times |
| Debug button | Toggle mine visibility |

## Building

### Prerequisites

- A C++11 compiler (g++)
- [SFML 2.x](https://www.sfml-dev.org/) installed on your system (`sfml-graphics`, `sfml-window`, `sfml-system`, `sfml-audio`)

### Compile & Run

```bash
make build
./minesweeper
```

## Project Structure

```
├── src/           # Source files
│   ├── main.cpp        # Game loop, windows, input handling
│   ├── board.cpp       # Minesweeper grid logic
│   ├── tile.cpp        # Individual tile state & rendering
│   ├── bullethell.cpp  # Bullet-hell attack patterns & spawning
│   ├── bullet.cpp      # Bullet physics & collision detection
│   ├── UI.cpp          # HUD sprites (counter, timer, face, buttons)
│   ├── timer.cpp       # Game timer
│   └── extra.cpp       # Utility helpers
├── include/       # Header files
├── files/
│   ├── images/         # Tile, UI, and bullet sprites
│   ├── sounds/         # Bullet & death sound effects
│   ├── config.cfg      # Board dimensions & mine count
│   ├── font.ttf        # UI font
│   └── leaderboard.txt # Persistent top-5 leaderboard
└── Makefile
```

## Configuration

Edit `files/config.cfg` to change the board size and mine count:

```
25 16 50
```

Format: `columns  rows  mines`

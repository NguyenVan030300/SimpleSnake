# Snake

A small terminal-based Snake game written in C++.

## Requirements

- CMake 3.14 or newer
- A C++17 compiler
- Linux or macOS terminal
- Internet access during the first CMake configure, to download GoogleTest

## Build

From the project root, run:

```sh
cmake -S . -B build
cmake --build build
```

## Play

Run the game with:

```sh
./build/snakeProject
```

- Use the arrow keys to move.
- Eat `*` to gain 10 points.
- Press `q` to quit.
- Hitting a wall or the snake's body ends the game.
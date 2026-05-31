# Arcade

A modular **gaming platform** written in **C++**, where both the **graphics backend** and the **games** are interchangeable **shared libraries** loaded at runtime. From the in-app menu you can switch the renderer (e.g. ncurses → SDL2) or the game **on the fly**, without ever restarting the program.

The whole point of the project is a strict separation of concerns: a game never knows which graphics library draws it, and a graphics library never knows which game it renders. They only ever talk through small, well-defined interfaces.

## Features

- **Runtime plugin architecture** — graphics and game modules are compiled as `.so` libraries and loaded dynamically (`dlopen`); the core knows only the `IDisplay` / `IGame` interfaces
- **Hot-swappable backends** — change the graphics library or the game at runtime from the menu, keeping the rest of the platform untouched
- **Three graphics backends** — **ncurses**, **SDL2** and **Allegro5**, each behind the same display interface (text and graphical rendering handled uniformly)
- **Two games** — **Snake** and **Solarfox**, fully decoupled from rendering
- **Shared menu & scores** — a graphical menu to pick game/backend, player name and persistent high scores
- **Asset abstraction** — sprites/fonts are described once and rendered appropriately by each backend (a textured sprite in SDL2, a character in ncurses)
- **Tested & CI** — unit tests and GitHub Actions workflows

## Build

Requires CMake, a C++ compiler, and the graphics libraries you want to use (`ncurses`, `SDL2`, `Allegro5`).

```bash
cmake -B build
cmake --build build
```

This produces the `arcade` binary and the graphics/game modules as shared libraries in `lib/`.

## Usage

```bash
./arcade ./lib/arcade_ncurses.so
```

Pass the path to an initial **graphics** library. Once running, use the menu to choose a game and switch graphics backends or games at any time.

## Credits

Team project realized as part of the Epitech curriculum — **OOP** module.

- Nielsen Combe-Bracciale
- Mathys Aberkane

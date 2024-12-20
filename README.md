# Game of Life

Welcome to **Game of Life**, a Conway's Game of Life implementation created by **Dimitrios Rammos** as part of the **Data Structures** course in the **Department of Informatics and Telecommunications (DIT)** at the **University of Athens**. This project is developed in **C** and demonstrates advanced usage of data structures and algorithms.


If you want learn for Game-Of-Life read [here](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).</br>
Check [here](https://conwaylife.com/wiki/Category:Patterns_found_by_(person)) for patterns.</br>
The official page by [Conway's Game of Life](https://conwaylife.com/).</br>


---

## 📜 Introduction

The **Game of Life** is a cellular automaton devised by the mathematician John Conway. It's a zero-player game, meaning its evolution is determined by its initial state without requiring further input. This implementation allows users to input patterns in the `.rle` format and outputs the evolution of the game as a GIF animation.


---

## 🛠️ Project Structure

### Directory Overview

- **files/**: Contains the input `.rle` files representing initial patterns for the Game of Life, including well-known patterns such as:
  - `glider-train.rle`
  - `hebdarole.rle`
  - `pentoad.rle`
  - `ship-in-a-bottle.rle`
  - `breeder-1.rle`
  - `gospergundestroyedbygliders.rle`
  - `new-gun-1.rle`
  - `p24shuttleshasslingoctagon2.rle`
  - `p29pentadecathlonhassler.rle`
  - `ship-in-a-bottle.rle`
  - `two-glider.rle`

- **include/**: Header files for modularization, including:
  - `ADTList.h`, `ADTMap.h`, `ADTVector.h`: Custom data structure implementations.
  - `bmp.h`, `gif.h`: Helpers for generating output images and animations.
  - `LifeState.h`: Core game logic definitions.

- **modules/**: C source files implementing the logic of the headers in `include/`.

- **program/**: Contains the main program logic:
  - `lifegame.c`: Main entry point for the application.
  - `Makefile`: Build instructions.
  - `output.gif`: Generated GIF of the game’s evolution.

---

## 📑 Features

### 🌟 Core Features:
- **Input Patterns in RLE Format**: Supports `.rle` input files with initial patterns of the Game of Life.
- **GIF Animation Output**: Generates an animated GIF (`output.gif`) showcasing the evolution of the game.
- **Custom Data Structures**: Implements efficient data structures (e.g., lists, maps, vectors) for managing game states.
- **Modular Code**: Separated into reusable modules and libraries.

### 🔧 Supported Patterns:
- Predefined patterns such as gliders, guns, oscillators, and more.
- Examples available in the `files/` directory.

### 🚀 Performance:
- Uses optimized algorithms and custom data structures for efficient processing of large game grids.

---

## 📦 Requirements

To build and run the project, you need:

- A Unix-based operating system (tested on Ubuntu via WSL).
- A C compiler (e.g., GCC).
- **Make** build system.

---

## 🚀 Usage

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/drammos/Game-Of-Life.git
cd Game-Of-Life
```

### 2️⃣ Navigate to the program Directory
```bash
cd program
```

### 3️⃣ Build and Run the Game
Run the following command to compile and execute:
```bash
make run
```

---

## 🔧 Input/Output Description

### Input:
- Input files are located in the `files/` directory.
- Each file is in `.rle` (Run-Length Encoded) format, which describes initial patterns for the game grid.

### Output:
- A single file named `output.gif` is generated, which visualizes the evolution of the game.

---

## 🗂️ Code Highlights

### `LifeState.c`
The core logic of the Game of Life, including:
- **Grid Initialization:** Reads `.rle` files to initialize the game grid.
- **Rules Implementation:** Applies Conway's rules to compute the next generation.
- **State Management:** Tracks and updates cells efficiently using custom data structures.


### `LifeGame.c`
The main entry point for the program:
- **Input Handling:** Reads user-specified input patterns.
- **Game Execution:** Executes the game logic using `LifeState`.
- **Output Generation:** Outputs the results as an animated GIF.

---


## 🧠 About the Developer

Developed by **Dimitrios Rammos**, a passionate student of Computer Science at the **University of Athens**. This project showcases his expertise in:
- **Data Structures and Algorithms.**
- **Systems Programming in C.**
- **Modular Software Development.**


---

## 📬 Contact

For any questions or suggestions, feel free to contact the author:

- **GitHub:** [drammos](https://github.com/drammos)  
- **LinkedIn:** [Dimitrios Rammos](https://www.linkedin.com/in/dimitrisrammos/)

# Sudoku
A simple TUI and GUI soduku game.


## TUI
The TUI game requires Bash to display the characters properly. It works by entering the coordinate of a square first, and then the digit you wish to enter in that square. 
It can be compiled by simply compiling the `sudoku.c` file.

## GUI
The GUI game requires the [TIGR library](https://github.com/erkkah/tigr), eg `gcc .\sudokuGUI.c .\tigr.c -lopengl32 -lgdi32 -o sudokuGUI`.
You use the mouse to choose a difficulty, and then use the keyboard to navigate the cursor on the board, pressing the digit keys will edit the number in that cell, assuming it can be edited.
You use the mouse to use the `exit` and `reset` buttons on the left.

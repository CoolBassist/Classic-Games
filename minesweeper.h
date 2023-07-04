#pragma once

#include <stdbool.h>
#include <time.h>

typedef struct{
    int mines_around;
    bool is_mine;
    bool is_discovered;
    bool is_flagged;
} MS_Cell;

struct{
    int board_w;
    int board_h;
    int start_x;
    int start_y;
    int no_mines;

    int width;
    int height;

    int square_side;

    time_t time_started;
    bool game_started;
    bool game_over;
    bool has_clicked;

    MS_Cell* board;
}MS_INFO;


void MS_new_game(int mines, int width, int height);
void MS_menu(Tigr* screen);
MS_Cell* MS_getCell(int x, int y);
void MS_discover(int x, int y);
void MS_generateBoard();
void MS_resetBoard();
void MS_convert_click(int* x, int* y);
int MS_count_flags(int x, int y);
void MS_rgtclk(int x, int y);

bool MS_has_won();
void MS_drawUI(Tigr* screen);

void MS_discover(int x, int y);
void MS_flag(int x, int y);

bool MS_is_valid_mouse_click();

void MS_drawField(Tigr* screen);

int MS_find_mines_around(int x, int y);

void play_minesweeper(Tigr* screen);
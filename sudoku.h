#pragma once

#include <stdbool.h>
#include "tigr.h"

enum difficulty{
    SU_EASY = 21,
    SU_MEDIUM = 32,
    SU_HARD = 43,
    S0_VERYHARD = 64
};

typedef struct{
    int value;
    int is_editable;
} SU_Cell;

struct{
    SU_Cell board[9][9];
    int already_used[9][9][9];
    bool game_started;
    int board_w;
    int start_x;
    int start_y;
    int cur_x;
    int cur_y;
    int delta; 
    int side_length; 
    int hl_digit; 
    time_t time_started;
    bool game_over;
} SU_INFO;

void SU_menu(Tigr* screen);

void SU_new_game(int diff);
void play_sudoku(Tigr* screen);

void SU_generate_board();
void SU_remove_squares(int toRemove);

void draw_board(Tigr* screen);
void SU_drawUI(Tigr* screen);
void SU_draw_numbers(Tigr* screen);

bool SU_verify_row(int y, int d);
bool SU_verify_column(int x, int d);
bool SU_verify_square(int s, int d);
bool SU_verify_full_board();

bool SU_has_won();
bool SU_is_valid_mouse_click();
void SU_convert_click(int* x, int* y);
void SU_interpret_key(int key);
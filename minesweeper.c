#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gc.h"
#include "minesweeper.h"
#include "tigr.h"


void MS_menu(Tigr* screen){
    Button* easy_diff_button = createButton("EASY", TIGR_DRKBLU, 130, 70);
    Button* med_diff_button = createButton("MED", TIGR_DRKBLU, 130, 100);
    Button* hard_diff_button = createButton("HARD", TIGR_DRKBLU, 130, 130);

    Button* back_button = createButton("BACK", TIGR_LGTRED, 130, 200);

    MS_INFO.board_w = 181;
    MS_INFO.start_x = 77;
    MS_INFO.start_y = 30;
    MS_INFO.game_over = false;
    MS_INFO.has_clicked = false;
    MS_INFO.game_started = true;

    while(true){
        tigrClear(screen, TIGR_BCKGRO);
        int title_screen_offset = (320 - tigrTextWidth(TITLE_FONT, "MINESWEEPER"))/2;
        tigrPrint(screen, TITLE_FONT, title_screen_offset, 30, TIGR_DRKBLU, "MINESWEEPER");
        drawButton(screen, easy_diff_button);
        drawButton(screen, med_diff_button);
        drawButton(screen, hard_diff_button);
        drawButton(screen, back_button);

        resetMouse();
        getMouseClick(screen);

        if(isButtonPressed(easy_diff_button)){
            MS_new_game(10, 9, 9);
            play_minesweeper(screen);
        }
        if(isButtonPressed(med_diff_button)){
            MS_new_game(40, 15, 14);
            play_minesweeper(screen);
        }
        if(isButtonPressed(hard_diff_button)){
            MS_new_game(99, 20, 19);
            play_minesweeper(screen);
        }

        if(isButtonPressed(back_button)){
            break;
        }

        tigrUpdate(screen);
    }
}

void MS_new_game(int mines, int width, int height){
    MS_INFO.square_side = 181/width;
    MS_INFO.board_h = (181/width)*height;
    MS_INFO.time_started = time(NULL);
    MS_INFO.no_mines = mines;

    MS_INFO.board = malloc(width*height*sizeof(MS_Cell));
    MS_INFO.width = width;
    MS_INFO.height = height;
    MS_INFO.has_clicked = false;
    MS_INFO.game_over = false;

    MS_resetBoard();
}

void MS_drawField(Tigr* screen){
    tigrLine(screen, MS_INFO.start_x-1, MS_INFO.start_y-1, MS_INFO.start_x+MS_INFO.board_w, MS_INFO.start_y-1, TIGR_BLK); // TOP
    tigrLine(screen, MS_INFO.start_x-1, MS_INFO.start_y+MS_INFO.board_h, MS_INFO.start_x+MS_INFO.board_w, MS_INFO.start_y+MS_INFO.board_h, TIGR_BLK); // BOTTOM

    tigrLine(screen, MS_INFO.start_x-1, MS_INFO.start_y, MS_INFO.start_x-1, MS_INFO.start_y + MS_INFO.board_h, TIGR_BLK); // LEFT
    tigrLine(screen, MS_INFO.start_x+MS_INFO.board_w-1, MS_INFO.start_y, MS_INFO.start_x+MS_INFO.board_w-1, MS_INFO.start_y + MS_INFO.board_h, TIGR_BLK); // RIGHT


    for(int y = 0; y < MS_INFO.height; y++){
        for(int x = 0; x < MS_INFO.width; x++){
            TPixel cur_colour = TIGR_LGTPUR;
            MS_Cell* cur_cell = MS_getCell(x, y);

            if(cur_cell->is_discovered && cur_cell->is_mine) cur_colour = TIGR_LGTRED;
            else if(cur_cell->is_flagged) cur_colour = TIGR_DRKPUR;
            else if(!cur_cell->is_discovered) cur_colour = TIGR_DRKBLU;

            tigrFillRect(screen, MS_INFO.start_x+x*MS_INFO.square_side, MS_INFO.start_y+y*MS_INFO.square_side, MS_INFO.square_side, MS_INFO.square_side, cur_colour);

            if(cur_cell->is_discovered && !cur_cell->is_mine){
                if(cur_cell->mines_around != 0){
                    char n[2];
                    sprintf(n, "%d", cur_cell->mines_around);

                    int x_offset = (MS_INFO.square_side - tigrTextWidth(SMALL_FONT, n))/2;
                    int y_offset = (MS_INFO.square_side - tigrTextHeight(SMALL_FONT, n))/2;

                    TPixel colours[9] = {
                        TIGR_DRKBLU, // blue
                        tigrRGB(0x20, 0xbf, 0x55), // green
                        tigrRGB(0xef, 0x47, 0x6f), // red
                        tigrRGB(0x26, 0x54, 0x7c), // dark blue
                        tigrRGB(0x91, 0x17, 0x1f), // maroon
                        TIGR_BLK, // black
                        tigrRGB(0x84, 0x8c, 0x8e)  // grey
                    };

                    tigrPrint(screen, SMALL_FONT, MS_INFO.start_x+MS_INFO.square_side*x+x_offset, MS_INFO.start_y+MS_INFO.square_side*y+y_offset, colours[cur_cell->mines_around-1], n);
                }
            }
        }
    }
}

void play_minesweeper(Tigr* screen){
    Button* exit_button = createButton("EXIT", TIGR_LGTRED, 10, 32);
    while(true){
        tigrClear(screen, TIGR_BCKGRO);

        drawButton(screen, exit_button);

        MS_drawField(screen);
        MS_drawUI(screen);

        resetMouse();
        getMouseClick(screen);

        if((!MS_has_won() && !MS_INFO.game_over) && MS_is_valid_mouse_click()){
            int x, y;

            MS_convert_click(&x, &y);

            if(!MS_INFO.has_clicked){
                MS_INFO.has_clicked = true;
                MS_generateBoard(x, y);
            }

            if(MOUSE.button == 1){
                MS_rgtclk(x, y);
            }
            if(MOUSE.button == 4){
                MS_flag(x, y);
            }
        }

        if(MS_has_won()){
            int x_offset = (320-tigrTextWidth(tfont, "B )"))/2;
            tigrPrint(screen, tfont, x_offset, 15, TIGR_DRKBLU, "B )");
        }else if(MS_INFO.game_over){
            int x_offset = (320-tigrTextWidth(tfont, ": ("))/2;
            tigrPrint(screen, tfont, x_offset, 15, TIGR_DRKBLU, ": (");
        }else{
            int x_offset = (320-tigrTextWidth(tfont, ": )"))/2;
            tigrPrint(screen, tfont, x_offset, 15, TIGR_DRKBLU, ": )");
        }

        if(isButtonPressed(exit_button)){
            break;
        }

        tigrUpdate(screen);
    }
}

void MS_rgtclk(int x, int y){
    MS_Cell* cur_cell = MS_getCell(x, y);

    if(!cur_cell->is_discovered){
        MS_discover(x, y);
        return;
    }

    if(MS_count_flags(x, y) == cur_cell->mines_around){
        for(int i = (y == 0 ? 0 : -1); i < (y == MS_INFO.height-1 ? 1 : 2); i++){
            for(int j = (x == 0 ? 0 : -1); j < (x == MS_INFO.width-1 ? 1 : 2); j++){
                if(i == 0 && j == 0) continue;
                MS_discover(x+j, y+i);
            }
        }
    }
}

MS_Cell* MS_getCell(int x, int y){
    return &(MS_INFO.board[y*MS_INFO.width + x]);
}

bool MS_is_valid_mouse_click(){
    bool x = MOUSE.x >= MS_INFO.start_x && MOUSE.x <= MS_INFO.start_x + MS_INFO.board_w;
    bool y = MOUSE.y >= MS_INFO.start_y && MOUSE.y <= MS_INFO.start_y + MS_INFO.board_h;

    return x && y;
}

void MS_resetBoard(){
    for(int y = 0; y < MS_INFO.height; y++){
        for(int x = 0; x < MS_INFO.width; x++){
            MS_Cell* cur_cell = MS_getCell(x, y);
            cur_cell->is_discovered = false;
            cur_cell->is_flagged = false;
            cur_cell->is_mine = false;
            cur_cell->mines_around = 0;
        }
    }
}

void MS_convert_click(int* x, int* y){
    *x = ((MOUSE.x - MS_INFO.start_x)/MS_INFO.square_side);
    *y = ((MOUSE.y - MS_INFO.start_y)/MS_INFO.square_side);

   // printf("MOUSE: %d %d\n", *x, *y);
}

void MS_drawUI(Tigr* screen){
    /* Mines left */
    char textMinesLeft[3];
    sprintf(textMinesLeft, "%d", MS_INFO.no_mines);
    tigrPrint(screen, tfont, 290, 35, TIGR_DRKBLU, textMinesLeft);
    tigrFillRect(screen, 265, 30, 20, 20, TIGR_LGTRED);

    /* timer */
    char timeCount[5];
    static time_t cur_time;
    if((!MS_has_won()) && (!MS_INFO.game_over)){
        cur_time = time(NULL) - MS_INFO.time_started;
    }
     
    sprintf(timeCount, "%d", cur_time);
    tigrPrint(screen, tfont, 290, 76, TIGR_DRKBLU, timeCount);

    tigrCircle(screen, 275, 80, 10, TIGR_BLK);
    tigrFillCircle(screen, 275, 80, 10, TIGR_WHITE);

    tigrLine(screen, 275, 80, 275+sin(cur_time*0.10472)*10, 80-cos(cur_time*0.10472)*10, TIGR_BLK);
}

void MS_discover(int x, int y){
    MS_Cell* cur_cell = MS_getCell(x, y);
    if(cur_cell->is_discovered) return;
    if(cur_cell->is_flagged) return;

    cur_cell->is_discovered = true;

    if(cur_cell->is_mine){
        MS_INFO.game_over = true;

        for(int y2 = 0; y2 < MS_INFO.height; y2++){
            for(int x2 = 0; x2 < MS_INFO.width; x2++){
                MS_Cell* temp_cell = MS_getCell(x2, y2);
                if(temp_cell->is_mine && !temp_cell->is_flagged) temp_cell->is_discovered = true;
            }
        }

        return;
    }

    if(cur_cell->mines_around == 0){
        for(int i = (y == 0 ? 0 : -1); i < (y == MS_INFO.height-1 ? 1 : 2); i++){
            for(int j = (x == 0 ? 0 : -1); j < (x == MS_INFO.width-1 ? 1 : 2); j++){
                if(i == 0 && j == 0) continue;
                MS_discover(x+j, y+i);
            }
        }
    }
}

int MS_count_flags(int x, int y){
    int flags_counted = 0;
    for(int i = (y == 0 ? 0 : -1); i < (y == MS_INFO.height-1 ? 1 : 2); i++){
        for(int j = (x == 0 ? 0 : -1); j < (x == MS_INFO.width-1 ? 1 : 2); j++){
            if(i == 0 && j == 0) continue;
            MS_Cell* cur_cell = MS_getCell(x+j, y+i);

            if(cur_cell->is_flagged) flags_counted++;
        }
    }

    return flags_counted;
}

void MS_flag(int x, int y){
    MS_Cell* cur_cell = MS_getCell(x, y);
    if(cur_cell->is_discovered) return;

    if(cur_cell->is_flagged){
        MS_INFO.no_mines++;
    }else{
        MS_INFO.no_mines--;
    }

    cur_cell->is_flagged = !cur_cell->is_flagged;
}

void MS_generateBoard(int x, int y){
    int rand_x;
    int rand_y;
    int mines_left = MS_INFO.no_mines;

    while(mines_left != 0){
        rand_x = rand()%MS_INFO.width;
        rand_y = rand()%MS_INFO.height;

        MS_Cell* cur_cell = MS_getCell(rand_x, rand_y);

        if(!cur_cell->is_mine && rand_x != x && rand_y != y){
            cur_cell->is_mine = true;
            mines_left--;
        }
    }

    for(int y = 0; y < MS_INFO.height; y++){
        for(int x = 0; x < MS_INFO.width; x++){
            MS_Cell* current_cell = MS_getCell(x, y);
            current_cell->mines_around = MS_find_mines_around(x, y);
        }
    }
}

bool MS_has_won(){
    for(int y = 0; y < MS_INFO.height; y++){
        for(int x = 0; x < MS_INFO.width; x++){
            MS_Cell* cur_cell = MS_getCell(x, y);
            if(!cur_cell->is_discovered && !cur_cell->is_mine) return false;
        }
    }

    for(int y = 0; y < MS_INFO.height; y++){
        for(int x = 0; x < MS_INFO.width; x++){
            MS_Cell* cur_cell = MS_getCell(x, y);
            if(cur_cell->is_mine){
                cur_cell->is_flagged = true;
            }
        }
    }

    MS_INFO.no_mines = 0;
    return true;
}

int MS_find_mines_around(int x, int y){
    size_t minesAround = 0;
    for(int i = (y == 0 ? 0 : -1); i < (y == MS_INFO.height-1 ? 1 : 2); i++){
        for(int j = (x == 0 ? 0 : -1); j < (x == MS_INFO.width-1 ? 1 : 2); j++){
            if(i == 0 && j == 0) continue;
            MS_Cell* cur_cell = MS_getCell(x+j, y+i);
            if(cur_cell->is_mine){
                minesAround++;
            }
        }
    }

    return minesAround;
}

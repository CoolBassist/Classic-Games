#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "sudoku.h"
#include "gc.h"
#include "tigr.h"


void SU_menu(Tigr* screen){
    Button* easy_diff_button = createButton("EASY", TIGR_DRKBLU, 130, 70);
    Button* med_diff_button = createButton("MED", TIGR_DRKBLU, 130, 100);
    Button* hard_diff_button = createButton("HARD", TIGR_DRKBLU, 130, 130);
    Button* very_hard_diff_button = createButton("V.HARD", TIGR_DRKBLU, 130, 160);

    Button* back_button = createButton("BACK", TIGR_LGTRED, 130, 200);

    while(true){
        tigrClear(screen, TIGR_BCKGRO);
        int title_screen_offset = (320 - tigrTextWidth(TITLE_FONT, "SUDOKU"))/2;
        tigrPrint(screen, TITLE_FONT, title_screen_offset, 30, TIGR_DRKBLU, "SUDOKU");
        drawButton(screen, easy_diff_button);
        drawButton(screen, med_diff_button);
        drawButton(screen, hard_diff_button);
        drawButton(screen, very_hard_diff_button);

        drawButton(screen, back_button);

        resetMouse();
        getMouseClick(screen);

        if(isButtonPressed(easy_diff_button)){
            SU_new_game(SU_EASY);
            play_sudoku(screen);
        }
        if(isButtonPressed(med_diff_button)){
            SU_new_game(SU_MEDIUM);
            play_sudoku(screen);
        }
        if(isButtonPressed(hard_diff_button)){
            SU_new_game(SU_HARD);
            play_sudoku(screen);
        }
        if(isButtonPressed(very_hard_diff_button)){
            SU_new_game(S0_VERYHARD);
            play_sudoku(screen);
        }

        if(isButtonPressed(back_button)){
            break;
        }

        tigrUpdate(screen);
    }
}

void SU_new_game(int difficulty){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            SU_INFO.board[j][i].value = 0;
            SU_INFO.board[j][i].is_editable = false;
        }
    }

    memset(SU_INFO.already_used, 0, 9*9*9*sizeof(int));

    SU_generate_board();
    SU_remove_squares(difficulty);

    SU_INFO.game_started = true;
    SU_INFO.board_w = 181;
    SU_INFO.start_x = 77;
    SU_INFO.start_y = 30;
    SU_INFO.delta = 60;
    SU_INFO.hl_digit = 0;
    SU_INFO.cur_x = -1;
    SU_INFO.cur_y = -1;
    SU_INFO.game_over = false;
    SU_INFO.time_started = time(NULL);
}

void SU_generate_board(){
    int curDigit;

    int y = 0;
    int x = 0;

    while(y != 9){
        while(x != 9){
            if(y < 0){
                printf("Error Y is less than 0!\n");
                exit(0);
            }

            if(x < 0){
                printf("Error X is less than 0!\n");
                exit(0);
            }

            int tries[9];

            for(int i = 0; i < 9; i++){
                tries[i] = SU_INFO.already_used[y][x][i];
            }

            bool triedAll;

            do{
                triedAll = true;
                for(int i = 0; i < 9; i++){
                    if(tries[i] == 0){
                        triedAll = false;
                    }
                }

                if(triedAll){
                    SU_INFO.board[y][x].value = 0;
                    memset(&SU_INFO.already_used[y][x], 0, 9*sizeof(int));
                    break;
                }

                curDigit = (rand() % 9) + 1;
                while((tries[curDigit-1] != 0) || (SU_INFO.already_used[y][x][curDigit-1] != 0)){
                    curDigit = (rand() % 9) + 1;
                }

                tries[curDigit-1] = 1;                
                SU_INFO.board[y][x].value = curDigit;

            }while(!SU_verify_row(y, curDigit) || !SU_verify_column(x, curDigit) || !SU_verify_square((x/3) + (y/3)*3, curDigit));

            if(triedAll){
                --x;
            }else{
                SU_INFO.already_used[y][x][curDigit-1] = 1;
                ++x;
            }
        }
        x = 0;
        ++y;
    }
}

void SU_remove_squares(int to_remove){
    int randX;
    int randY;

    while(to_remove != 0){
        do{
            randX = (rand()%9);
            randY = (rand()%9);
        }while(SU_INFO.board[randY][randX].value == 0);

        SU_INFO.board[randY][randX].value = 0;
        SU_INFO.board[randY][randX].is_editable = true;
        --to_remove;
    }
}

void SU_draw_board(Tigr* screen){
    for(int i = 0; i < 4; i++){
        tigrLine(screen, SU_INFO.start_x, SU_INFO.start_y + i*SU_INFO.delta, SU_INFO.start_x+SU_INFO.board_w, SU_INFO.start_y + i*SU_INFO.delta, TIGR_BLK);
    }

    for(int i = 0; i < 4; i++){
        tigrLine(screen, SU_INFO.start_x+i*SU_INFO.delta, SU_INFO.start_y , SU_INFO.start_x+i*SU_INFO.delta, SU_INFO.board_w+SU_INFO.start_y, TIGR_BLK);
    }

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){

            if(SU_INFO.cur_x == x && SU_INFO.cur_y == y){
                tigrFillCircle(screen, x*20+SU_INFO.start_x+10, y*20+10 + SU_INFO.start_y, 10, TIGR_HNYDEW);
            }

            if(!SU_INFO.board[y][x].is_editable){
                if(SU_INFO.board[y][x].value != SU_INFO.hl_digit){
                    tigrFillCircle(screen, x*20+SU_INFO.start_x+10, y*20+10 + SU_INFO.start_y, 10, tigrRGB(0x9A, 0xC2, 0xC5));
                }else if(SU_INFO.hl_digit != 0){
                    tigrFillCircle(screen, x*20+SU_INFO.start_x+10, y*20+10 + SU_INFO.start_y, 10, tigrRGB(0x36, 0x26, 0xA7));
                }
            }else if(SU_INFO.board[y][x].value == SU_INFO.hl_digit && SU_INFO.hl_digit != 0){
                tigrFillCircle(screen, x*20+SU_INFO.start_x+10, y*20+10 + SU_INFO.start_y, 10, tigrRGB(0x65, 0x7E, 0xD4));
            }   
        }
    }
}

void SU_draw_numbers(Tigr* screen){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){

            if(SU_INFO.board[y][x].value == 0) continue;

            char cell_number[2];
            sprintf(cell_number, "%d", SU_INFO.board[y][x].value);

            TPixel colour = tigrRGB(0x80, 0xEE, 0xFF);

            if(SU_INFO.cur_x == x && SU_INFO.cur_y == y){
                colour = tigrRGB(0xFF, 0xFF, 0xFF);
            }        

            if(!SU_verify_row(y, SU_INFO.board[y][x].value) || !SU_verify_column(x, SU_INFO.board[y][x].value) || !SU_verify_square((x/3) + (y/3)*3, SU_INFO.board[y][x].value)){
                if(SU_INFO.board[y][x].is_editable){
                    colour = tigrRGB(0xEF, 0x76, 0x74);
                }else{
                    colour = tigrRGB(0xC4, 0x23, 0x48);
                }
            }

            tigrPrint(screen, tfont, SU_INFO.start_x + 20*x + 7, SU_INFO.start_y + 20*y + 7, colour, cell_number);          
        }
    }
}

bool SU_verify_row(int y, int d){
    int count = 0;

    for(int i = 0; i < 9; i++){
        if(SU_INFO.board[y][i].value == d) count++;
    }

    return count < 2;
}

bool SU_verify_column(int x, int d){
    int count = 0;

    for(int i = 0; i < 9; i++){
        if(SU_INFO.board[i][x].value == d) count++;
    }

    return count < 2;
}

bool SU_verify_square(int s, int d){
    int count = 0;

    int x = s % 3;
    int y = s / 3;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(SU_INFO.board[i+3*y][j+3*x].value == d) count++;
        }
    }

    return count < 2;
}

bool SU_verify_full_board(){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(SU_INFO.board[y][x].value == 0) continue;

            if(!SU_verify_column(x, SU_INFO.board[y][x].value)) return false;
            if(!SU_verify_row(y, SU_INFO.board[y][x].value)) return false;
            if(!SU_verify_square((x/3) + (y/3)*3, SU_INFO.board[y][x].value)) return false;
        } 
    }

    return true;
}

void play_sudoku(Tigr* screen){
    Button* exit_button = createButton("EXIT", TIGR_LGTRED, 10, 32);
    while(true){
        tigrClear(screen, TIGR_BCKGRO);

        drawButton(screen, exit_button);

        SU_draw_board(screen);
        SU_draw_numbers(screen);
        SU_drawUI(screen);

        resetMouse();
        getMouseClick(screen);

        int key_pressed = tigrReadChar(screen);
        if(key_pressed != 0) SU_interpret_key(key_pressed);

        if((!SU_has_won() && !SU_INFO.game_over) && SU_is_valid_mouse_click()){
            int x, y;

            SU_convert_click(&x, &y);

            if(MOUSE.button == 1){

                if(SU_INFO.cur_x == x && SU_INFO.cur_y == y){
                    SU_INFO.cur_x = -1;
                    SU_INFO.cur_y = -1;
                }else{
                    SU_INFO.cur_x = x;
                    SU_INFO.cur_y = y;
                }

                if(SU_INFO.board[y][x].value != 0){
                    SU_INFO.hl_digit = SU_INFO.board[y][x].value;
                }else if(SU_INFO.hl_digit == SU_INFO.board[y][x].value){
                    SU_INFO.hl_digit = 0;
                }
            }
        }

        if(SU_has_won()){
            SU_INFO.cur_x = -1;
            SU_INFO.cur_y = -1;
            SU_INFO.hl_digit = 0;
        }

        if(isButtonPressed(exit_button)){
            break;
        }

        tigrUpdate(screen);
    }
}

bool SU_has_won(){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(SU_INFO.board[y][x].value == 0) return false;
        }
    }

    return SU_verify_full_board();
}

void SU_convert_click(int* x, int* y){
    *x = (MOUSE.x - SU_INFO.start_x)/20;
    *y = (MOUSE.y - SU_INFO.start_y)/20;
}

bool SU_is_valid_mouse_click(){
    bool x = MOUSE.x >= SU_INFO.start_x && MOUSE.x <= SU_INFO.start_x + SU_INFO.board_w;
    bool y = MOUSE.y >= SU_INFO.start_y && MOUSE.y <= SU_INFO.start_y + SU_INFO.board_w;

    return x && y;
}

void SU_drawUI(Tigr* screen){
    char timeCount[5];
    static time_t cur_time;
    if((!SU_has_won()) && (!SU_INFO.game_over)){
        cur_time = time(NULL) - SU_INFO.time_started;
    }
     
    sprintf(timeCount, "%d", cur_time);
    tigrPrint(screen, tfont, 290, 76, TIGR_DRKBLU, timeCount);

    tigrCircle(screen, 275, 80, 10, TIGR_BLK);
    tigrFillCircle(screen, 275, 80, 10, TIGR_WHITE);

    tigrLine(screen, 275, 80, 275+sin(cur_time*0.10472)*10, 80-cos(cur_time*0.10472)*10, TIGR_BLK);
}

void SU_interpret_key(int key){
    SU_INFO.hl_digit = 0;

    if(key >= '0' && key <= '9'){
        int cur_num = key - 48;
        if(SU_INFO.board[SU_INFO.cur_y][SU_INFO.cur_x].is_editable){
            if(SU_INFO.board[SU_INFO.cur_y][SU_INFO.cur_x].value == cur_num){
                SU_INFO.board[SU_INFO.cur_y][SU_INFO.cur_x].value = 0;
                SU_INFO.hl_digit = 0;
            }else{
                SU_INFO.board[SU_INFO.cur_y][SU_INFO.cur_x].value = cur_num;
                SU_INFO.hl_digit = cur_num;
            }
        }else{
            SU_INFO.hl_digit = cur_num;
        }
    }

    if(key == 8){ // backspace
        SU_INFO.board[SU_INFO.cur_y][SU_INFO.cur_x].value = 0;
        SU_INFO.hl_digit = 0;
    }
}
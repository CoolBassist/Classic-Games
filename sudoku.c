#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "tigr.h"

/*** Enums ***/
enum difficulty{
    EASY = 21,
    MEDIUM = 32,
    HARD = 43,
    VERYHARD = 64
};

/*** Prototypes ***/
void drawBoard();

/*** Structs ***/
typedef struct {
    int value;
    bool isEditable;
} cell;

/**** Variables ****/
cell board[9][9];
int alreadyUsed[9][9][9];
char message[100];
Tigr *screen;
int cur_x;
int cur_y;
int delta = 60;
int start_x = 77;
int start_y = 30;
int cell_offset = 7;
int side_length = 181;
int timeStart;
int difficulty;
int hlDigit;
bool start = false;

/**** Verification ****/
bool verifyRow(int y, int d){
    int count = 0;

    for(int i = 0; i < 9; i++){
        if(board[y][i].value == d) count++;
    }

    return count < 2;
}

bool verifyColumn(int x, int d){
    int count = 0;

    for(int i = 0; i < 9; i++){
        if(board[i][x].value == d) count++;
    }

    return count < 2;
}

bool verifySquare(int s, int d){
    int count = 0;

    int x = s % 3;
    int y = s / 3;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board[i+3*y][j+3*x].value == d) count++;
        }
    }

    return count < 2;
}

bool verifyFullBoard(){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(board[y][x].value == 0) continue;

            if(!verifyColumn(x, board[y][x].value)) return false;
            if(!verifyRow(y, board[y][x].value)) return false;
            if(!verifySquare((x/3) + (y/3)*3, board[y][x].value)) return false;
        } 
    }

    return true;
}


/**** Generation of puzzle ****/
void generateFullBoard(){
    int curDigit;

    int y = 0;
    int x = 0;

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            board[j][i].value = 0;
        }
    }

    memset(alreadyUsed, 0, 9*9*9*sizeof(int));


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

            board[y][x].isEditable = false;

            int tries[9];

            for(int i = 0; i < 9; i++){
                tries[i] = alreadyUsed[y][x][i];
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
                    board[y][x].value = 0;
                    memset(&alreadyUsed[y][x], 0, 9*sizeof(int));
                    break;
                }

                curDigit = (rand() % 9) + 1;
                while((tries[curDigit-1] != 0) || (alreadyUsed[y][x][curDigit-1] != 0)){
                    curDigit = (rand() % 9) + 1;
                }

                tries[curDigit-1] = 1;                
                board[y][x].value = curDigit;

            }while(!verifyRow(y, curDigit) || !verifyColumn(x, curDigit) || !verifySquare((x/3) + (y/3)*3, curDigit));

            if(triedAll){
                --x;
            }else{
                alreadyUsed[y][x][curDigit-1] = 1;
                ++x;
            }
        }
        x = 0;
        ++y;
    }
}

void removeSquares(int toRemove){
    int randX;
    int randY;

    while(toRemove != 0){
        do{
            randX = (rand()%9);
            randY = (rand()%9);
        }while(board[randY][randX].value == 0);

        board[randY][randX].value = 0;
        board[randY][randX].isEditable = true;
        --toRemove;
    }
}


/**** Input ****/
void interpretKey(int key){
    hlDigit = 0;

    if(key >= '0' && key <= '9'){
        int cur_num = key - 48;
        if(board[cur_y][cur_x].isEditable){
            if(board[cur_y][cur_x].value == cur_num){
                board[cur_y][cur_x].value = 0;
                hlDigit = 0;
            }else{
                board[cur_y][cur_x].value = cur_num;
                hlDigit = cur_num;
            }
        }else{
            hlDigit = cur_num;
        }
    }

    if(key == 8){ // backspace
        board[cur_y][cur_x].value = 0;
        hlDigit = 0;
    }
}


/*** Drawing ***/
void drawBoard(){
    TPixel line_colour = tigrRGB(0x22, 0x18, 0x1C);

    for(int i = 0; i < 4; i++){
        tigrLine(screen, start_x, start_y + i*delta, side_length+start_x, start_y + i*delta, line_colour);
    }

    for(int i = 0; i < 4; i++){
        tigrLine(screen, start_x+i*delta, start_y , start_x+i*delta, side_length+start_y, line_colour);
    }

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(!board[y][x].isEditable){
                if(board[y][x].value != hlDigit){
                    tigrFillCircle(screen, x*20+start_x+10, y*20+10 + start_y, 10, tigrRGB(0x9A, 0xC2, 0xC5));
                }else if(hlDigit != 0){
                    tigrFillCircle(screen, x*20+start_x+10, y*20+10 + start_y, 10, tigrRGB(0x36, 0x26, 0xA7));
                }
            }else if(board[y][x].value == hlDigit && hlDigit != 0){
                tigrFillCircle(screen, x*20+start_x+10, y*20+10 + start_y, 10, tigrRGB(0x65, 0x7E, 0xD4));
            }   
        }
    }
}

void drawNumbers(){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){

            if(board[y][x].value == 0) continue;

            char cell_number[2];
            sprintf(cell_number, "%d", board[y][x].value);

            TPixel colour = tigrRGB(0x80, 0xEE, 0xFF);

            if(cur_x == x && cur_y == y){
                colour = tigrRGB(0xFF, 0xFF, 0xFF);
            }        

            if(!verifyRow(y, board[y][x].value) || !verifyColumn(x, board[y][x].value) || !verifySquare((x/3) + (y/3)*3, board[y][x].value)){
                if(board[y][x].isEditable){
                    colour = tigrRGB(0xEF, 0x76, 0x74);
                }else{
                    colour = tigrRGB(0xC4, 0x23, 0x48);
                }
            }

            tigrPrint(screen, tfont, start_x + 20*x + cell_offset, start_y + 20*y + cell_offset, colour, cell_number);          
        }
    }
}

void drawCursor(){
    tigrFillCircle(screen, cur_x*20+start_x+10, cur_y*20+10 + start_y, 10, tigrRGB(0x07, 0xA0, 0xC3));
}

void drawButton(char* text, int x, int y){
    tigrFillRect(screen, x, y, 60, 20, tigrRGB(0x07, 0xA0, 0xC3));
    tigrFillRect(screen, x-2, y-2, 60, 20, tigrRGB(0x80, 0xEE, 0xFF));
    tigrPrint(screen, tfont, x+2, y+3, tigrRGB(0xFF, 0xFF, 0xFF), text);
}

void drawUI(){
    char timeCount[30];
    char diff[8];

    sprintf(timeCount, "%ld SECONDS", time(NULL)-timeStart);

    if(difficulty == EASY){
        memcpy(diff, "EASY", 5);
    }else if(difficulty == MEDIUM){
        memcpy(diff, "MEDIUM", 7);
    }else if(difficulty == HARD){
        memcpy(diff, "HARD", 5);
    }else if(difficulty == VERYHARD){
        memcpy(diff, "V. HARD", 8);
    }

    int count[9] = {0};

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            count[board[y][x].value - 1]++;
        }
    }

    char t_s[2];
    for(int i = 0; i < 9; i++){
        sprintf(t_s, "%d", i+1);

        tigrPrint(screen, tfont, 280, 29+22*i, tigrRGBA(0x07, 0xa0, 0xc3, 255-28*count[i]), t_s);
    }


    tigrPrint(screen, tfont, 80, 10, tigrRGB(0xEF, 0x76, 0x74), diff);
    tigrPrint(screen, tfont, 140, 10, tigrRGB(0xEF, 0x76, 0x74), timeCount);

    drawButton("RESET", 10, 32);
    drawButton("BACK", 10, 62);
}

void drawMenu(){
    tigrPrint(screen, tfont, 137, 20, tigrRGB(0xEF, 0x76, 0x74), "SUDOKU");
    drawButton("EASY", 137, 40);
    drawButton("MEDIUM", 137, 70);
    drawButton("HARD", 137, 100);
    drawButton("V. HARD", 137, 130);

    drawButton("EXIT", 137, 200);
}


/*** Game functions ***/
bool hasWon(){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(board[y][x].value == 0){
                return false;
            }
        }
    }

    return verifyFullBoard();
}

bool isButtonPressed(int t_x, int t_y, int x, int y){
    return t_x > x && t_x < x+60 && t_y > y && t_y < y+20;
}

/*** Main ***/

int main(){
    srand((unsigned int)time(NULL));
    screen = tigrWindow(320, 240, "Sudoku", 0);
    cur_x = 0;
    cur_y = 0;
    int x = 0;
    int y = 0;

    int prev_time = 0;

    int butt_prev;
    int butt = 0;
    
    while(!tigrClosed(screen)){
        tigrClear(screen, tigrRGB(0xDF, 0xF8, 0xEB));

        butt_prev = butt;
        tigrMouse(screen, &x, &y, &butt);

        if(start){
            drawUI();
            drawBoard();
            drawCursor();
            drawNumbers();

            

            if(butt != 0 && butt != butt_prev){
                if(isButtonPressed(x, y, 10, 32)){
                    generateFullBoard();
                    removeSquares(difficulty);
                    timeStart = time(NULL);
                }else if(isButtonPressed(x, y, 10, 62)){
                    start = false;
                }
            }


            if(tigrKeyDown(screen, TK_DOWN)){
                if(cur_y < 8){
                    cur_y += 1;
                }
            }
            if(tigrKeyDown(screen, TK_UP)){
                if(cur_y > 0){
                    cur_y -= 1;
                }
            }
            if(tigrKeyDown(screen, TK_RIGHT)){
                if(cur_x < 8){
                    cur_x += 1;
                }
            }
            if(tigrKeyDown(screen, TK_LEFT)){
                if(cur_x > 0){
                    cur_x -= 1;
                }
            }

            int key_pressed = tigrReadChar(screen);

            if(key_pressed != 0) interpretKey(key_pressed);

            if(hasWon()){
                start = 0;
                prev_time = time(NULL) - timeStart;
            }
        }else{
            drawMenu();

            if(butt != 0 && butt != butt_prev){
                if(isButtonPressed(x, y, 137, 40)){
                    start = true;
                    difficulty = EASY;
                }else if(isButtonPressed(x, y, 137, 70)){
                    start = true;
                    difficulty = MEDIUM;
                }else if(isButtonPressed(x, y, 137, 100)){
                    start = true;
                    difficulty = HARD;
                }else if(isButtonPressed(x, y, 137, 130)){
                    start = true;
                    difficulty = VERYHARD;
                }else if(isButtonPressed(x, y, 137, 200)){
                    break;
                }
            }

            if(prev_time != 0){
                char time_str[30];
                sprintf(time_str, "PREV TIME: %d", prev_time);
                tigrPrint(screen, tfont, 30, 20, tigrRGB(0xFF, 0xFF, 0xFF), time_str);
            }

            if(start){
                timeStart = time(NULL);
                generateFullBoard();
                removeSquares(difficulty);
            }
        }

        tigrUpdate(screen);
    }

    tigrFree(screen);
    return 0;
}
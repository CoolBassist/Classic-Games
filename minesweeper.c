#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "tigr.h"


/*** Defines ***/
#define max(x,y) (((x) >= (y)) ? (x) : (y))
#define min(x,y) (((x) <= (y)) ? (x) : (y))


/*** Structs ***/
typedef struct {
    bool isMine;
    bool isFlagged;
    bool isDiscovered;
    int surroundingMines;
}cell;

/*** Variables ***/
Tigr* screen;
cell* board;
int width;
int height;
bool isOver = false;
int minesLeft;
char face[3];
bool started = false;

/*** Helpers ***/
cell* get(int x, int y){
    return &board[y*9 + x];
}

void findMinesAround(int x, int y){
    size_t minesAround = 0;
    for(int i = (y == 0 ? 0 : -1); i < (y == 8 ? 1 : 2); i++){
        for(int j = (x == 0 ? 0 : -1); j < (x == 8 ? 1 : 2); j++){
            if(i == 0 && j == 0) continue;
            cell* curCell = get(x+j, y+i);
            if((*curCell).isMine){
                minesAround++;
            }
        }
    }

    cell* cur_cell = get(x, y);

    (*cur_cell).surroundingMines = minesAround;
}


/*** Field Generation ***/
void clearBoard(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            cell* curCell = get(i, j);
            (*curCell).isDiscovered = false;
            (*curCell).isFlagged = false;
            (*curCell).isMine = false;
            (*curCell).surroundingMines = 0;
        }
    }
}

void generateField(int x, int y){
    int start_x = 77;
    int start_y = 30;
    int delta = 20;

    int newX = x-start_x;
    int newY = y-start_y;

    if(newY < 0 || newX < 0) return;

    newX = (x-start_x)/delta;
    newY = (y-start_y)/delta;

    if(newY > 8 || newX > 8) return;


    size_t minesLeft = 10;
    int rand_x;
    int rand_y;

    clearBoard();

    while(minesLeft != 0){
        rand_x = rand()%9;
        rand_y = rand()%9;

        cell* currentCell = get(rand_x, rand_y);

        if(!(*currentCell).isMine && rand_x != newX && rand_y != newY){
            (*currentCell).isMine = true;
            minesLeft--;
        }
    }

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            findMinesAround(j, i);
        }
    }

    started = true;
}

/*** Drawing ***/
void drawBoard(){
    int delta = 60;
    int start_x = 77;
    int start_y = 30;
    int cell_offset = 7;
    int side_length = 181;

    tigrLine(screen, start_x-2, start_y-2, start_x+side_length, start_y-2, tigrRGB(0x22, 0x18, 0x1C)); // top
    tigrLine(screen, start_x-2, start_y+side_length, start_x+side_length, start_y+side_length, tigrRGB(0x22, 0x18, 0x1C)); // bottom
    tigrLine(screen, start_x-2, start_y-2, start_x-2, start_y+side_length, tigrRGB(0x22, 0x18, 0x1C)); // left
    tigrLine(screen, start_x+side_length, start_y-2, start_x+side_length, start_y+side_length+1, tigrRGB(0x22, 0x18, 0x1C)); // right
    
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            cell* curCell = get(x, y);
            
            if((*curCell).isDiscovered){
                if((*curCell).isMine){
                    tigrFillRect(screen, start_x+20*x, start_y+20*y, 20, 20, tigrRGB(0xEF, 0x76, 0x74));
                }else{
                    tigrFillRect(screen, start_x+20*x, start_y+20*y, 20, 20, tigrRGB(0x65, 0x7E, 0xD4));
                    if((*curCell).surroundingMines != 0){
                        char n[2];
                        sprintf(n, "%d", (*curCell).surroundingMines);
                        tigrPrint(screen, tfont, start_x+20*x+7, start_y+20*y+7, tigrRGB(0xFF, 0xFF, 0xFF), n);
                    }
                }
            }else if((*curCell).isFlagged){
                tigrFillRect(screen, start_x+20*x, start_y+20*y, 20, 20, tigrRGB(0x36, 0x26, 0xA7));
            }else{
                tigrFillRect(screen, start_x+20*x, start_y+20*y, 20, 20, tigrRGB(0x07, 0xA0, 0xC3));
            }
        }
    }
}

void drawButton(char* text, int x, int y){
    tigrFillRect(screen, x, y, 60, 20, tigrRGB(0x07, 0xA0, 0xC3));
    tigrFillRect(screen, x-2, y-2, 60, 20, tigrRGB(0x80, 0xEE, 0xFF));
    tigrPrint(screen, tfont, x+2, y+3, tigrRGB(0xFF, 0xFF, 0xFF), text);
}

void drawUI(){
    char textMinesLeft[3];
    sprintf(textMinesLeft, "%d mine%s left", minesLeft, minesLeft == 1 ? "" : "s");
    tigrPrint(screen, tfont, 70, 15, tigrRGB(0x07, 0xA0, 0xC3), textMinesLeft);
    tigrPrint(screen, tfont, 200, 15, tigrRGB(0x07, 0xA0, 0xC3), face);
    drawButton("RESET", 10, 32);
    tigrPrint(screen, tfont, 120, 220, tigrRGB(0xEF, 0x76, 0x74), "MINESWEEPER");
}



/*** Game Functions ***/
void discover(int x, int y){
    cell* curCell = get(x, y);
    if((*curCell).isDiscovered) return;

    (*curCell).isDiscovered = true;

    if((*curCell).isMine){
        isOver = true;
        memcpy(face, ": (", 3);  
        return;
    }

    if((*curCell).surroundingMines == 0){
        for(int i = (y == 0 ? 0 : -1); i < (y == 8 ? 1 : 2); i++){
            for(int j = (x == 0 ? 0 : -1); j < (x == 8 ? 1 : 2); j++){
                if(i == 0 && j == 0) continue;
                discover(x+j, y+i);
            }
        }
    }
}

size_t surroundingFlags(int x, int y){
    size_t flagsCounted = 0;

    for(int i = (y == 0 ? 0 : -1); i < (y == 8 ? 1 : 2); i++){
        for(int j = (x == 0 ? 0 : -1); j < (x == 8 ? 1 : 2); j++){
            if(i == 0 && j == 0) continue;
            cell* curCell = get(x+j, y+i);
            if((*curCell).isFlagged) flagsCounted++;
        }
    }

    return flagsCounted;
}

void mouseClick(int x, int y, int button){
    int start_x = 77;
    int start_y = 30;
    int delta = 20;

    int newX = x-start_x;
    int newY = y-start_y;

    if(newY < 0 || newX < 0) return;

    newX = (x-start_x)/delta;
    newY = (y-start_y)/delta;

    if(newY > 8 || newX > 8) return;

    cell* curCell = get(newX, newY);

    if(button == 1){
        if((*curCell).isFlagged) return;
        
        if((*curCell).isDiscovered){
            size_t flagsCounted = surroundingFlags(newX, newY);

            if(flagsCounted == (*curCell).surroundingMines){
                for(int i = (newY == 0 ? 0 : -1); i < (newY == 8 ? 1 : 2); i++){
                    for(int j = (newX == 0 ? 0 : -1); j < (newX == 8 ? 1 : 2); j++){
                        if(i == 0 && j == 0) continue;
                        cell* tempCell = get(newX+j, newY+i);
                        if(!(*tempCell).isFlagged) discover(newX+j, newY+i);
                    }
                }
            }
        }else{
            discover(newX, newY);
        }

        
    };

    if(button == 4){
        if((*curCell).isDiscovered) return;
        (*curCell).isFlagged = !(*curCell).isFlagged;
        if((*curCell).isFlagged){
            minesLeft--;
        }else{
            minesLeft++;
        }
    }
}

bool isButtonPressed(int t_x, int t_y, int x, int y){
    return t_x > x && t_x < x+60 && t_y > y && t_y < y+20;
}

void newGame(){
    isOver = false;
    started = false;
    minesLeft = 10;
    memcpy(face, ": )", 3);
    clearBoard();
}

bool hasWon(){
    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            cell* curCell = get(x, y);
            if(!(*curCell).isDiscovered && !(*curCell).isMine) return false;
        }
    }

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            cell* curCell = get(x, y);
            if((*curCell).isMine){
                (*curCell).isFlagged = true;
            }
        }
    }

    minesLeft = 0;
    return true;
}


/*** Main ***/

int main(void){
    srand((unsigned int)time(NULL));
    screen = tigrWindow(320, 240, "Minesweeper", 0);

    board = malloc(9*9*sizeof(cell));
    
    int butt_prev;
    int butt = 0;
    int x = 0;
    int y = 0;
    
    newGame();

    while(!tigrClosed(screen)){
        tigrClear(screen, tigrRGB(0xDF, 0xF8, 0xEB));
        drawBoard();
        drawUI();

        

        butt_prev = butt;
        tigrMouse(screen, &x, &y, &butt);

        if(!isOver){
            if(butt != 0 && butt != butt_prev){
                if(!started){
                    generateField(x, y);
                }
                mouseClick(x, y, butt);
            }
        }

        if(butt != 0 && butt != butt_prev){
            if(isButtonPressed(x, y, 10, 32)){
                newGame();
            }
        }

        if(hasWon() && started){
            isOver = true;
            memcpy(face, "B )", 3);
        }

        tigrUpdate(screen);
    }

    tigrFree(screen);
    free(board);

    return 0;
}
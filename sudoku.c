#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/**** defines ****/
#define GREEN_FG "\e[0;32m"
#define RED_BG   "\e[0;91m"
#define RESET    "\e[0m"

#define changeStyle(x) printf(x)

typedef struct {
    int pencilMarks[9];
    int value;
    bool filledIn;
    bool isEditable;
} cell;





void drawBoard();

/**** variables ****/
cell board[9][9];
int alreadyUsed[9][9][9];
bool pencil = false;
char message[100];
int move[2];

/**** verification ****/
bool verifyRow(int r, int d){
    int count = 0;

    for(int i = 0; i < 9; i++){
        if(board[r][i].value == d) count++;
    }

    return count < 2;
}

bool verifyColumn(int c, int d){
    int count = 0;

    for(int i = 0; i < 9; i++){
        if(board[i][c].value == d) count++;
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

/**** generation of puzzle ****/
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

            board[y][x].isEditable = true;
            board[y][x].filledIn = true;

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

int solveBoard(){
    int* freeSquares[81];
    int numberOfFreeSquares = 0;

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(board[y][x].value == 0){
                freeSquares[numberOfFreeSquares] = &board[y][x].value;
                numberOfFreeSquares++;
            }
        }
    }

    int* tried = (int*) malloc(numberOfFreeSquares*9*sizeof(int));
    int solutions = 0;

    memset(&tried, 0, numberOfFreeSquares*9*sizeof(int));

    do{
        for(int i = 0; i < numberOfFreeSquares; i++){
            for(int j = 1; j <= 9; j++){

                bool repeat = true;

                for(int k = 0; k < 9; k++){
                    for(int l = 0; l < 9; l++){
                        if(tried[k*9 + l] == 0)  repeat =  false;
                    }
                }

                if(repeat) return solutions;



                if(tried[i*9 + (j-1)] == 0){
                    *freeSquares[i] = j;
                }else{
                    continue;
                }
                tried[i*9 + (j-1)] = 1;
                if(!verifyFullBoard){
                    continue;
                }
            }
        }

        solutions++;
    }while(true);

    return solutions;
}

void removeSquares(int toRemove){
    int randX;
    int randY;

    while(toRemove != 0){
        do{
            randX = (rand()%9);
            randY = (rand()%9);
        }while(board[randY][randX].value == 0);

        int oldValue = board[randY][randX].value;

        board[randY][randX].value = 0;

        --toRemove;
    }

    for(int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            if(board[y][x].value != 0){
                board[y][x].isEditable = false;
            }
        }
    }
}



/**** output ****/
void drawBoard(){
    printf("  012 345 678\n ╔═══╤═══╤═══╗\n");

    for(int y = 0; y < 9; y++){
        if(y%3 != 0 || y == 0)
            printf("%d║", y);
        else{
            printf(" ╟");
        }
        for(int x = 0; x < 9; x++){
            if(x % 3 == 0){
                if(x != 0)
                    printf("│");
            }
            if(y % 3 == 0){
                if(y != 0 && x == 0)
                    printf("───┼───┼───╢\n%d║", y);
            }

            if(!verifyColumn(x, board[y][x].value)){
                changeStyle(RED_BG);
            }

            if(!verifyRow(y, board[y][x].value)){
                changeStyle(RED_BG);
            }

            if(!board[y][x].isEditable){
                changeStyle(GREEN_FG);
            }

            if(board[y][x].filledIn && board[y][x].value != 0){
                printf("%d", board[y][x].value);
            }else{
                printf(" ");
            }

            changeStyle(RESET);
        }
        printf("║\n");
    }

    printf(" ╚═══╧═══╧═══╝\n");
}

/**** input ****/
bool getSquare(){
    move[0] = 0;
    move[1] = 0;

    char* buf;
    size_t limit = 100;

    buf = (char*) malloc(limit);

    if(buf == NULL) return false;

    long int bytesRead = getline(&buf, &limit, stdin);

    if(bytesRead == -1) return false;

    buf[bytesRead-1] = '\0';

    if(strcmp(buf, "pencil") == 0){
        move[0] = -1;
        move[1] = -1;

        return true;
    }

    char* pch;

    pch = strtok(buf, " ");

    move[0] = atoi(pch);
    pch = strtok(NULL, " ");
    move[1] = atoi(pch);

    if(move[0] > 8 || move[0] < 0 || move[1] > 8 || move[1] < 0) return false;

    free(buf);

    return board[move[1]][move[0]].isEditable;
}

int getNumber(){
    char* buf;
    size_t limit = 100;
    buf = (char*) malloc(limit);

    if(buf == NULL) return false;

    long int bytesRead = getline(&buf, &limit, stdin);

    if(bytesRead == -1) return false;

    buf[bytesRead-1] = '\0';

    char* pch;
    pch = strtok(buf, " ");
    int d = atoi(pch);

    if(d > 9 || d < 0) return false;

    free(buf);

    return d;
}

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

int main(){
    srand((unsigned int)time(NULL));
    generateFullBoard();
    removeSquares(26);
    memcpy(message, "Welcome to Sudoku!\n", 100);

    while(true){
        system("clear");
        printf("%s", message);
        drawBoard();
        bool succeed = getSquare();
        if(!succeed) continue;

        if(move[0] + move[1] == -2){ // pencil
            pencil = !pencil;
            if(pencil){
                memcpy(message, "Pencil mode activated.\n", 100);
            }else{
                memcpy(message, "Pencil mode deactivated.\n", 100);
            }
            continue;
        }

        snprintf(message, 100, "Selected square %d %d\n", move[0], move[1]);
        system("clear");
        printf("%s", message);
        drawBoard();

        board[move[1]][move[0]].value = getNumber();

        memcpy(message, "Select a square.\n", 100);

        if(hasWon()) break;
    }

    system("clear");
    memcpy(message, "You've won!\n", 100);
    printf("%s", message);
    drawBoard();


    return 0;
}
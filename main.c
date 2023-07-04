#include <stdio.h>
#include "gc.h"
#include "minesweeper.h"
#include "sudoku.h"
#include "tigr.h"

int main(int argc, char *argv[])
{
    Tigr *screen = tigrWindow(320, 240, "Game Collection", 0);

    TITLE_FONT_IMAGE = tigrLoadImage("16x16.png");
    TITLE_FONT = tigrLoadFont(TITLE_FONT_IMAGE, 0);

    SMALL_FONT_IMAGE = tigrLoadImage("8x8.png");
    SMALL_FONT = tigrLoadFont(SMALL_FONT_IMAGE, 0);

    Button* play_mine_button = createButton("M.SWEEP.", TIGR_DRKBLU, 60, 70);
    Button* play_sodoku_button = createButton("SUDOKU", TIGR_DRKBLU, 130, 70);
    Button* play_mancala_button = createButton("MANCALA", TIGR_LGTGREY, 200, 70);

    Button* play_hanoi_button = createButton("HANOI", TIGR_LGTGREY, 60, 100);
    Button* play_test12_button = createButton("TEST12", TIGR_LGTGREY, 130, 100);
    Button* play_test13_button = createButton("TEST13", TIGR_LGTGREY, 200, 100);

    Button* play_test21_button = createButton("TEST21", TIGR_LGTGREY, 60, 130);
    Button* play_test22_button = createButton("TEST22", TIGR_LGTGREY, 130, 130);
    Button* play_test23_button = createButton("TEST23", TIGR_LGTGREY, 200, 130);

    Button* exitProgButton = createButton("EXIT", TIGR_LGTRED, 130, 200);

    while (!tigrClosed(screen))
    {
        tigrClear(screen, TIGR_BCKGRO);
        int title_screen_offset = (320 - tigrTextWidth(TITLE_FONT, "GAME COLLECTION"))/2;
        tigrPrint(screen, TITLE_FONT, title_screen_offset, 30, TIGR_DRKBLU, "GAME COLLECTION");
        drawButton(screen, play_mine_button);
        drawButton(screen, play_sodoku_button);
        drawButton(screen, play_mancala_button);

        drawButton(screen, play_hanoi_button);
        drawButton(screen, play_test12_button);
        drawButton(screen, play_test13_button);

        drawButton(screen, play_test21_button);
        drawButton(screen, play_test22_button);
        drawButton(screen, play_test23_button);

        resetMouse();
        getMouseClick(screen);

        if(isButtonPressed(exitProgButton)){
            break;
        }

        if(isButtonPressed(play_mine_button)){
            MS_menu(screen);
        }

        if(isButtonPressed(play_sodoku_button)){
            SU_menu(screen);
        }



        drawButton(screen, exitProgButton);
        tigrUpdate(screen);
    }
    tigrFreeFont(TITLE_FONT);
    tigrFreeFont(SMALL_FONT);
    tigrFree(screen);
    return 0;
}


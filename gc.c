#include <stdlib.h>
#include <stdio.h>
#include "tigr.h"
#include "gc.h"

Button* createButton(char* text, TPixel colour, int x_pos, int y_pos){
    Button* button = malloc(sizeof(Button));
    button->text = text;
    button->x_pos = x_pos;
    button->y_pos = y_pos;
    button->colour = colour;

    return button;
}

void drawButton(Tigr* screen, const Button* button){
    tigrFillRect(screen, button->x_pos, button->y_pos, 60, 20, TIGR_DRKGREY);
    tigrFillRect(screen, button->x_pos-2, button->y_pos-2, 60, 20, button->colour);
    tigrPrint(screen, tfont, button->x_pos+2, button->y_pos+3, TIGR_WHITE, button->text);
}

bool isButtonPressed(const Button* button){
    bool x = (button->x_pos <= MOUSE.x) && (button->x_pos + 60 >= MOUSE.x);
    bool y = (button->y_pos <= MOUSE.y) && (button->y_pos + 20 >= MOUSE.y);

    return x && y;
}

void resetMouse(){
    MOUSE.x = 0;
    MOUSE.y = 0;
    MOUSE.button = 0;
}

bool getMouseClick(Tigr* screen){
    static int button_press = 0;
    static int prev_button_press;

    int x, y;
    prev_button_press = button_press;
    tigrMouse(screen, &x, &y, &button_press);

    if((button_press != 0) && (button_press != prev_button_press)){
        MOUSE.x = x;
        MOUSE.y = y;
        MOUSE.button = button_press;
        return true;
    }

    return false;
}
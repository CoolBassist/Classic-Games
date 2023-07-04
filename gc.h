#pragma once

#include <stdbool.h>
#include "tigr.h"

#define TIGR_BLK        tigrRGB(0x22, 0x18, 0x1c)
#define TIGR_DRKBLU     tigrRGB(0x07, 0xa0, 0xc3)
#define TIGR_LGTBLU     tigrRGB(0x80, 0xee, 0xff)
#define TIGR_LGTRED     tigrRGB(0xEF, 0x76, 0x74)
#define TIGR_LGTPUR     tigrRGB(0x65, 0x7E, 0xD4)
#define TIGR_DRKPUR     tigrRGB(0x36, 0x26, 0xA7)
#define TIGR_BCKGRO     tigrRGB(0xDF, 0xF8, 0xEB)
#define TIGR_WHITE      tigrRGB(0xF7, 0xF7, 0xFF)
#define TIGR_LGTGREY    tigrRGB(0x9E, 0xA3, 0xB0)
#define TIGR_DRKGREY    tigrRGB(0x39, 0x30, 0x53)
#define TIGR_HNYDEW     tigrRGB(0xB5, 0xC9, 0xBF)

Tigr* TITLE_FONT_IMAGE;
TigrFont* TITLE_FONT;

Tigr* SMALL_FONT_IMAGE;
TigrFont* SMALL_FONT;

typedef struct{
    char* text;
    int x_pos;
    int y_pos;
    TPixel colour;
}Button;

struct{
    int x;
    int y;
    int button;
}MOUSE;

/* Returns a pointer to a Button struct. */
Button* createButton(char* text, TPixel colour, int x_pos, int y_pos);

void drawButton(Tigr* screen, const Button* button);

bool isButtonPressed(const Button* button);

bool getMouseClick(Tigr* screen);

void resetMouse();
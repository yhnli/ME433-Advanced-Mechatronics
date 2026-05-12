#include "font.h"
#include "ssd1306.h"

// draw char c at (x,y) position
void drawChar(int x, int y, char c) {
    if (c < 0x20 || c > 0x7f) {
        c = 0x20;
    }

    int index = c - 0x20; 

    // each letter is 8 pixels tall, 5 pixels wide
    for (int col = 0; col < 5; col++) {
        char column_bits = ASCII[index][col];
        for (int row = 0; row < 8; row++) {
            int pixel = (column_bits >> row) & 0x01;
            ssd1306_drawPixel(x + col, y + row, pixel);
        }
    }
}

// void drawString(...);
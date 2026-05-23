#include "font.h"
#include "ssd1306.h"
#include <stdlib.h>

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

// draw string beginning at (x,y) position
void drawString(int x, int y, char* message) {
    int i = 0;
    while (message[i] != '\0') {
        drawChar(x + i * 6, y, message[i]);
        i++;
    }
}

// draw line from (x0,y0) to (x1,y1)
void drawLine(int x0, int y0, int x1, int y1, unsigned char color) {
    int dx  =  abs(x1 - x0);
    int sx  = (x0 < x1) ? 1 : -1;
    int dy  = -abs(y1 - y0);
    int sy  = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
 
    while (1) {
        if (x0 >= 0 && x0 < 128 && y0 >= 0 && y0 < 32) {
            ssd1306_drawPixel((unsigned char)x0, (unsigned char)y0, color);
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

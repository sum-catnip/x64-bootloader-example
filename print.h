#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

#include "memory.h"

#define VIDMEM_BASE 0xB8000
#define ROWS 25
#define COLS 80

#define DEF_BG blue
#define DEF_FG light_white

// enum of all the possible 26 bit colors
typedef enum { 
    black  = 0x0, 
    blue   = 0x1,
    green  = 0x2,
    aqua   = 0x3,
    red    = 0x4,
    purple = 0x5,
    yellow = 0x6,
    white  = 0x7,
    gray   = 0x8,
    
    light_blue   = 0x9,
    light_green  = 0xA,
    light_aqua   = 0xB,
    light_red    = 0xC,
    light_purple = 0xD,
    light_yellow = 0xE,
    light_white  = 0xF
} color;

// represents on character in the screen buffer
typedef struct {
    char character;
    // high is bg, low is fg
    uint8_t _color;
} __attribute__((packed)) vidchar;

// a pointer to the screen buffer
vidchar* vidmem_map = (vidchar*)VIDMEM_BASE;

// clears the whole screen with whitespace
// also sets the defaulf fg and bg
void clear_screen(color fg, color bg) {
    for(int i = 0; i < COLS * ROWS; i++) {
        //vidchar_setbg(&vidmem_map[i], bg);
        //vidchar_setfg(&vidmem_map[i], fg);
        vidmem_map[i]._color = (bg << 4) | fg;
        vidmem_map[i].character = ' ';
    }
}

// prints a new line
void printnl(void) {
    intptr_t x = ((intptr_t)vidmem_map - VIDMEM_BASE) / 2;
    vidmem_map += (COLS * ((x/COLS) +1)) - x;
}

// prints a single character, accepts \n for newline
// use -1 to not change the color (TODO, lol) 
void printch_col(char ch, color fg, color bg) {
    switch(ch) {
        case '\n': 
            printnl();
            break;

        default:
            vidmem_map->character = ch;
            vidmem_map->_color    = (bg << 4) | fg;
            ++vidmem_map;
            break;
    }
    // TODO: fix this shit you skid lol
    // if were at the end of the screen buffer
    int64_t vidmem_size = (ROWS * COLS) * sizeof(vidchar);
    if((int64_t)vidmem_map >= VIDMEM_BASE + vidmem_size) {
        vidmem_map -= COLS;

        memcpy(
            (uint8_t*)VIDMEM_BASE + (COLS * sizeof(vidchar)), 
            (uint8_t*)VIDMEM_BASE,
            vidmem_size - COLS
        );
        for(int i = 0; i < COLS; i++) {
            vidmem_map[i].character = ' ';
            vidmem_map[i]._color = (DEF_BG << 4) | DEF_FG;
        }
    }
}
void printch(char ch) { printch_col(ch, DEF_FG, DEF_BG); }

// prints a whole string, -1 to not change the color
void printstr_col(char* string, color fg, color bg) {
    while(*string != '\0') printch_col(*string++, fg, bg);
}
void printstr(char* string) { printstr_col(string, DEF_FG, DEF_BG); }

// prints 8bytes as hex
void printhex8_col(int64_t number, color fg, color bg) {
    const char hex_lookup[] = "0123456789ABCDEF";
    for(int i = (sizeof(number) * 8) -4; i >= 0; i -= 4)
        printch_col(hex_lookup[(number>>i) & 0xf], fg, bg);
}
void printhex8(int64_t number) { printhex8_col(number, DEF_FG, DEF_BG); }

// prints 4bytes as hex
void printhex4_col(int32_t number, color fg, color bg) {
    const char hex_lookup[] = "0123456789ABCDEF";
    for(int i = (sizeof(number) * 8) -4; i >= 0; i -= 4)
        printch_col(hex_lookup[(number>>i) & 0xf], fg, bg);
}
void printhex4(int32_t number) { printhex4_col(number, DEF_FG, DEF_BG); }

// prints 2bytes as hex
void printhex2_col(int16_t number, color fg, color bg) {
    const char hex_lookup[] = "0123456789ABCDEF";
    for(int i = (sizeof(number) * 8) -4; i >= 0; i -= 4)
        printch_col(hex_lookup[(number>>i) & 0xf], fg, bg);
}
void printhex2(int16_t number) { printhex2_col(number, DEF_FG, DEF_BG); }

// prints 1byte as hex
void printhex1_col(int8_t number, color fg, color bg) {
    const char hex_lookup[] = "0123456789ABCDEF";
    for(int i = (sizeof(number) * 8) -4; i >= 0; i -= 4)
        printch_col(hex_lookup[(number>>i) & 0xf], fg, bg);
}
void printhex1(int8_t number) { printhex1_col(number, DEF_FG, DEF_BG); }

// inits the screen buffer (clears the screen)
void init_screen() { clear_screen(DEF_FG, DEF_BG); }

#endif
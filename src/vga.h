#ifndef VGA_H
#define VGA_H

// 80x25

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#include <stdint.h>
#include <stddef.h>

struct vga {
	uint16_t* video_mem; // 0xB8000
	uint16_t row;
	uint16_t col;
};

void init_screen();
void xychar(int x, int y, char c, char color);
void perfectchar(char c, char color);
size_t strlen(const char* str);
void println(const char* str);
void clear();

#endif

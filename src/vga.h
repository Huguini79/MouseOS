#ifndef VGA_H
#define VGA_H

// 80x25

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define FONDO_AZUL 0x1F
#define FONDO_VERDE 0x2F
#define FONDO_ROJO 0x4F
#define FONDO_AMARILLO 0xEF

#include <stdint.h>
#include <stddef.h>

struct vga {
	uint16_t* video_mem; // 0xB8000
	uint16_t row;
	uint16_t col;
};

void init_screen();
void cursor_1(int mouse_x, int mouse_y);
void cursor_2(int mouse_x, int mouse_y);
void xychar(int x, int y, char c, char color);
void perfectchar(char c, char color);
size_t strlen(const char* str);
void println(const char* str);
void clear();

#endif

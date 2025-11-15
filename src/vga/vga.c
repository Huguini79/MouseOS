#include "vga.h"

#include <stdint.h>
#include <stddef.h>

struct vga* vga;

void init_screen() {
	vga->video_mem = (uint16_t*)(0xB8000);

	clear();

}

void xychar(int x, int y, char c, char color) {
	vga->video_mem[(y * VGA_WIDTH) + x] = (uint16_t)(color << 8) | c;
}

void perfectchar(char c, char color) {
	if(c == '\n') {
		vga->row += 1;
		vga->col = 0;
	}

	xychar(vga->col, vga->row, c, color);
	vga->col += 1;

	if(vga->col >= VGA_WIDTH) {
		vga->row += 1;
		vga->col = 0;
	}

	/*
	if(vga->row >= VGA_HEIGHT) {
		clear();
	}
	*/

}

size_t strlen(const char* str) {
	size_t len = 0;

	while(str[len]) {
		len++;
	}

	return len;

}

void println(const char* str) {
	size_t len = strlen(str);

	for(int i = 0; i < len; i++) {
		perfectchar(str[i], FONDO_AZUL);
	}

}

void clear() {
	vga->row = 0;
	vga->col = 0;

	for(int y = 0; y < VGA_HEIGHT; y++) {
		for(int x = 0; x < VGA_WIDTH; x++) {
			xychar(x, y, ' ', FONDO_AMARILLO);
		}
	}

}

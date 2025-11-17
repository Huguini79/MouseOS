#include "ps2.h"
#include "io.h"
#include "vga.h"

#include <stdint.h>
#include <stddef.h>

uint8_t mouse_cycle = 0;
int8_t  mouse_byte[3];

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08

const char* mouse_click_left_or_right;

int mouse_x = 40;
int mouse_y = 12;

void mouse_wait(uint8_t a_type) {
    uint32_t timeout = 100000;
    if (!a_type) {
        while (--timeout) {
            if ((insb(MOUSE_STATUS) & MOUSE_BBIT) == 1) return;
        }
    } else {
        while (--timeout) {
            if (!((insb(MOUSE_STATUS) & MOUSE_ABIT))) return;
        }
    }
}

void mouse_write(uint8_t write) {
    mouse_wait(1);
    outb(MOUSE_STATUS, MOUSE_WRITE);
    mouse_wait(1);
    outb(MOUSE_PORT, write);
}

uint8_t mouse_read() {
    mouse_wait(0);
    return insb(MOUSE_PORT);
}

void mouse_process_byte(uint8_t data) {
		cursor_1(mouse_x, mouse_y);
	    switch (mouse_cycle) {
		case 0:
		    mouse_byte[0] = data;
		    if (!(data & MOUSE_V_BIT)) return; // no es un paquete válido
		    mouse_cycle++;
		    break;
		case 1:
		    mouse_byte[1] = data;
		    mouse_cycle++;
		    break;
		case 2:
		    mouse_byte[2] = data;

		    // Ya tenemos un paquete completo
		    if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
		        // overflow, paquete inválido
		        mouse_cycle = 0;
		        return;
		    }

		    int dx = mouse_byte[1];
		    int dy = mouse_byte[2];

		    mouse_x += dx;
		    mouse_y -= dy; // Y invertido normalmente

		    // Limitar dentro de pantalla
		    if (mouse_x < 0) mouse_x = 0;
		    if (mouse_y < 0) mouse_y = 0;
		    if (mouse_x > 79) mouse_x = 79;
		    if (mouse_y > 24) mouse_y = 24;

		    // Dibujar cursor
		    cursor_2(mouse_x, mouse_y);

		    // Detectar clicks
		    if (mouse_byte[0] & 0x01) mouse_click_left_or_right = "Click izquierdo";
		    if (mouse_byte[0] & 0x02) mouse_click_left_or_right = "Click derecho";
		    if (mouse_byte[0] & 0x04) mouse_click_left_or_right = "Click central";

		    mouse_cycle = 0;
		    break;
	    }
	}

void keyboard_process_byte(uint8_t scancode) {
	if(scancode == 0x01) {
		println("ESC PRESIONADO");
	}
}

void mouse_install() {
    uint8_t status;
    mouse_wait(1);
    outb(MOUSE_STATUS, 0xA8);
    mouse_wait(1);
    outb(MOUSE_STATUS, 0x20);
    mouse_wait(0);
    status = insb(0x60) | 2;
    mouse_wait(1);
    outb(MOUSE_STATUS, 0x60);
    mouse_wait(1);
    outb(MOUSE_PORT, status);
    mouse_write(0xF6);
    mouse_read();
    mouse_write(0xF4);
    mouse_read();
}

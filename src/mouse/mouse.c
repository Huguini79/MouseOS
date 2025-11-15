#include "mouse.h"
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

// Variables globales para coordenadas absolutas
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

/* Polling handler: llámalo en tu bucle principal */
void mouse_handler() {
    uint8_t status = insb(MOUSE_STATUS);
    while (status & MOUSE_BBIT) {
    	xychar(mouse_x, mouse_y, ' ', 15);
        int8_t mouse_in = insb(MOUSE_PORT);
        if (status & MOUSE_F_BIT) {
            switch (mouse_cycle) {
                case 0:
                    mouse_byte[0] = mouse_in;
                    if (!(mouse_in & MOUSE_V_BIT)) return;
                    ++mouse_cycle;
                    break;
                case 1:
                    mouse_byte[1] = mouse_in;
                    ++mouse_cycle;
                    break;
                case 2:
                	// Detectar clicks
if (mouse_byte[0] & 0x01) {
    if(mouse_x == 0 & mouse_y == 0) {
    	println("Botón presionado");
    }
}
if (mouse_byte[0] & 0x02) {
}
if (mouse_byte[0] & 0x04) {
}

                    mouse_byte[2] = mouse_in;
                    if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) break;

                    int dx = mouse_byte[1];
                    int dy = mouse_byte[2];

                    mouse_x += dx;
                    mouse_y -= dy; // Y invertido normalmente

                    // Limitar dentro de pantalla
                    if (mouse_x < 0) mouse_x = 0;
                    if (mouse_y < 0) mouse_y = 0;
                    if (mouse_x > 79) mouse_x = 79;   // ancho VGA texto
                    if (mouse_y > 24) mouse_y = 24;   // alto VGA texto

                    xychar(mouse_x, mouse_y, '/', 15);

                    mouse_cycle = 0;
                    break;
            }
        }
        status = insb(MOUSE_STATUS);
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

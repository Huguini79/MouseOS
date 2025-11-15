#include "kernel.h"
#include "ps2.h"
#include "vga.h"
#include "io.h"

void kernel_main() {
	init_screen();

	println("BIENVENIDO A MouseOS");

	mouse_install();

	while (1) {
   		 uint8_t status = insb(0x64);
    		if (status & 0x01) { // hay datos en el buffer
        		uint8_t data = insb(0x60);
        		if (status & 0x20) {
            			// byte del ratón
            			mouse_process_byte(data);
        		} else {
            			// byte del teclado
            			keyboard_process_byte(data);
        		}
    		}
}


}

#include "kernel.h"
#include "mouse.h"
#include "vga.h"
#include "io.h"

void kernel_main() {
	init_screen();

	println("BIENVENIDO A MouseOS");

	mouse_install();

	while(1) {
		mouse_handler();
	}

}

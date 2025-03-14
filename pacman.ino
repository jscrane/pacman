/**
 * http://www.frisnit.com/pac-man-machine-emulator/
 * https://github.com/frisnit/pacman-emulator
 */
#include <stdarg.h>
#include <r65emu.h>
#include <z80.h>

#include "config.h"
#include "screen.h"
#include "io.h"

Memory memory;
z80 cpu(memory);
ram<> pages[2];

#include "roms/rom6e.h"
#include "roms/rom6f.h"
#include "roms/rom6h.h"
#include "roms/rom6j.h"

prom e6(rom6e, sizeof(rom6e));
prom f6(rom6f, sizeof(rom6f));
prom h6(rom6h, sizeof(rom6h));
prom j6(rom6j, sizeof(rom6j));

Screen screen(memory);
IO io(screen);
ps2_raw_kbd kbd(io);

void reset(void) {
	hardware_reset();
	kbd.reset();
	screen.begin();
}

void function_keys(uint8_t key) {
	switch (key) {
	case 1:
		reset();
		break;
	case 10:
		hardware_debug_cpu();
		break;
	}
}

void setup(void) {

	static uint8_t vec;

	cpu.set_port_out_handler([](uint16_t p, uint8_t b) {
		if ((p & 0xff) == 0x0000)
			vec = b;
	});

	hardware_init(cpu);

	hardware_interval_timer(16, []() { cpu.irq(vec); });

	memory.put(e6, 0x0000);
	memory.put(f6, 0x1000);
	memory.put(h6, 0x2000);
	memory.put(j6, 0x3000);

	memory.put(screen, 0x4000);
	memory.put(pages[0], 0x4800);
	memory.put(pages[1], 0x4c00);
	memory.put(io, 0x5000);

	reset();
}

void loop(void) {

	kbd.poll();

	if (!io.paused())
		hardware_run();
}

/**
 * http://www.frisnit.com/pac-man-machine-emulator/
 * https://github.com/frisnit/pacman-emulator
 */
#include <stdarg.h>
#include <r65emu.h>
#include <ports.h>
#include <z80.h>

#include "config.h"
#include "screen.h"
#include "io.h"

static uint8_t irq;

class IOPorts: public PortDevice<z80> {
public:
	uint8_t in(uint16_t p, z80 *cpu) {
		return 0;
	}

	void out(uint16_t p, uint8_t b, z80 *cpu) {
		if ((p & 0xff) == 0x0000)
			irq = b;
	}
} ports;

z80 cpu(memory, ports);
ram<> pages[2];

#include "roms/rom6e.h"
#include "roms/rom6f.h"
#include "roms/rom6h.h"
#include "roms/rom6j.h"

prom e6(rom6e, sizeof(rom6e));
prom f6(rom6f, sizeof(rom6f));
prom h6(rom6h, sizeof(rom6h));
prom j6(rom6j, sizeof(rom6j));

static bool paused;
Screen screen(memory);
IO io(screen);

void reset(void) {
	hardware_reset();
	screen.begin();
}

void setup(void) {
	hardware_init(cpu);

	memory.put(e6, 0x0000);
	memory.put(f6, 0x1000);
	memory.put(h6, 0x2000);
	memory.put(j6, 0x3000);

	memory.put(screen, 0x4000);
	memory.put(pages[0], 0x4800);
	memory.put(pages[1], 0x4c00);
	memory.put(io, 0x5000);

#if defined(DEBUGGING)
	Serial.begin(115200);
#endif

	reset();
}

void loop(void) {
	static bool debug;

	if (ps2.available()) {
		unsigned scan = ps2.read2();
		uint8_t key = scan & 0xff;
		if (is_down(scan))
			io.down(key);
		else
			switch (key) {
			case PS2_F1:
				reset();
				break;
			case PAUSE:
				paused = !paused;
				break;
			case PS2_F8:
				debug = !debug;
				break;
			default:
				io.up(key);
				break;
			}
	} else if (!paused) {
		cpu.run(1000);
		if (cpu.ts() > 51200) {
			cpu.reset_ts();
			if (io.int_enabled())
				cpu.raise(irq);
		}
#if defined(DEBUGGING)
		if (debug) {
			char buf[160];
			Serial.println(cpu.status(buf, sizeof(buf)));
		}
#endif
	}
}

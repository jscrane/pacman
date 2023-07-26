#include <Arduino.h>
#include <hardware.h>
#include <memory.h>
#include <display.h>

#include "config.h"
#include "screen.h"
#include "util/tiles_sprites.h"
#include "roms/rom82s123_7f.h"	// colours
#include "roms/rom82s126_4a.h"	// palette

void Screen::begin() {
	Display::begin(BLACK, WHITE, ORIENT, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	clear();
}

static void get_palette(palette_entry &p, uint8_t index) {
	index <<= 2;
	p.set_colour(colours[palette[index  ]], 0);
	p.set_colour(colours[palette[index+1]], 1);
	p.set_colour(colours[palette[index+2]], 2);
	p.set_colour(colours[palette[index+3]], 3);
}

void Screen::draw_tile(uint16_t t, int x, int y) {
	palette_entry p;
	get_palette(p, _tp[t + 0x0400]);

	uint8_t tile = _tp[t];
	const uint8_t *cdata = tiles + tile*64;
	for (int px = x; px < x+8; px++)
		for (int py = y; py < y+8; py++) {
			colour &c = p.colours[pgm_read_byte(cdata)];
			drawPixel(px, py, c.get());
			cdata++;
		}
}

void Screen::_set(uint16_t a, uint8_t b) {
	_tp[a] = b;
	if (a >= 0x400)
		a -= 0x400;
	int x = 0, y = 0;
	if (a >= 0x02 && a < 0x1e) {
		x = 27 - a+0x02;
		y = 34;
	} else if (a >= 0x22 && a < 0x3e) {
		x = 27 - a+0x22;
		y = 35;
	} else if (a >= 0x40 && a < 0x3c0) {
		uint16_t o = a - 0x40;
		x = 27 - o / 0x20;
		y = 2 + o % 0x20;
	} else if (a >= 0x3c2 && a < 0x3e0) {
		x = 27 - a+0x3c2;
		y = 0;
	} else if (a >= 0x3e2 && a < 0x400) {
		x = 27 - a+0x3e2;
		y = 1;
	}
	draw_tile(a, 8*x, 8*y);
}

void Screen::set_sprite(uint16_t off, uint8_t sx, uint8_t sy) {
	int x = DISPLAY_WIDTH - sx + 15;
	int y = DISPLAY_HEIGHT - sy - 16;

	palette_entry p;
	get_palette(p, _mem[0x4ff1 + off]);

	uint8_t sir = _mem[0x4ff0 + off];
	const uint8_t *cdata = sprites + 256*(sir >> 2);
	switch (sir & 0x03) {
	case 0:	// no flip
		for (int px = x; px < x+16; px++)
			for (int py = y; py < y+16; py++) {
				colour &c = p.colours[pgm_read_byte(cdata)];
				drawPixel(px, py, c.get());
				cdata++;
			}
		break;
	case 1:	// flip y
		for (int px = x; px < x+16; px++)
			for (int py = y + 15; py >= y; py--) {
				colour &c = p.colours[pgm_read_byte(cdata)];
				drawPixel(px, py, c.get());
				cdata++;
			}
		break;
	case 2:	// flip x
		for (int px = x+15; px >= x; px--)
			for (int py = y; py < y+16; py++) {
				colour &c = p.colours[pgm_read_byte(cdata)];
				drawPixel(px, py, c.get());
				cdata++;
			}
		break;
	case 3:	// flip x and y
		for (int px = x+15; px >= x; px--)
			for (int py = y+15; py >= y; py--) {
				colour &c = p.colours[pgm_read_byte(cdata)];
				drawPixel(px, py, c.get());
				cdata++;
			}
		break;
	}
}

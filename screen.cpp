#include <Arduino.h>
#include <machine.h>
#include <memory.h>
#include <hardware.h>
#include <debugging.h>
#include <display.h>

#include "config.h"
#include "screen.h"
#include "util/tiles_sprites.h"
#include "roms/rom82s123_7f.h"	// colours
#include "roms/rom82s126_4a.h"	// palette

static void get_palette(palette_entry &p, uint8_t index) {
	index <<= 2;
	p.set_colour(colours[palette[index  ]], 0);
	p.set_colour(colours[palette[index+1]], 1);
	p.set_colour(colours[palette[index+2]], 2);
	p.set_colour(colours[palette[index+3]], 3);
}

void Screen::begin() {

	_display.begin(BLACK, WHITE, ORIENT, DISPLAY_WIDTH, DISPLAY_HEIGHT, CENTER_ALL);
	_display.clear();

	for (int i = 0; i < 32; i++) {
		palette_entry p;
		get_palette(p, i);
		for (int j = 0; j < 4; j++)
			_palette565[i][j] = p.colours[j].get();
	}
}

void Screen::draw_tile(uint16_t t, int x, int y) {

	const uint8_t pindex = _tp[t + 0x0400] & 0x1f;
	const uint8_t *cdata = tiles + _tp[t] * 64;

	for (int px = x; px < x+8; px++)
		for (int py = y; py < y+8; py++) {
			_display.drawPixel(px, py, _palette565[pindex][pgm_read_byte(cdata)]);
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

	const uint8_t pindex = _mem[0x4ff1 + off];
	uint8_t sir = _mem[0x4ff0 + off];
	uint16_t sid = sir >> 2;
	const uint8_t *cdata = sprites + (sid << 8);

	switch (sir & 0x03) {
	case 0: // no flip
		for (int px = x; px <= x+15; px++)
			for (int py = y; py <= y+15; py++) {
				if (px >= 0 && px <= 223)
					_display.drawPixel(px, py, _palette565[pindex][pgm_read_byte(cdata)]);
				cdata++;
			}
		break;
	case 1: // flip y
		for (int px = x; px <= x+15; px++)
			for (int py = y+15; py >= y; py--) {
				_display.drawPixel(px, py, _palette565[pindex][pgm_read_byte(cdata)]);
				cdata++;
			}
		break;
	case 2: // flip x
		for (int px = x+15; px >= x; px--)
			for (int py = y; py <= y+15; py++) {
				if (px >= 0 && px <= 223)
					_display.drawPixel(px, py, _palette565[pindex][pgm_read_byte(cdata)]);
				cdata++;
			}
		break;
	}

	if (sid >= 44 && sid <= 48) {
		DBG_DSP("pacman at %d,%d flip=%d", x, y, sir & 0x03);

		static int opx, opy;

		int dx = x - opx;
		if (dx > 1)
			_display.drawFastVLine(x-1, y+2, 12, BLACK);
		else if (dx < -1)
			_display.drawFastVLine(x+16, y+2, 12, BLACK);
		opx = x;

		int dy = y - opy;
		if (dy > 1)
			_display.drawFastHLine(x+2, y-1, 12, BLACK);
		else if (dy < -1)
			_display.drawFastHLine(x+2, y+16, 12, BLACK);
		opy = y;
	}
}

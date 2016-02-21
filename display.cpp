#include <Energia.h>
#include <UTFT.h>
#include <r65emu.h>

#include "display.h"
#include "util/tiles_sprites.h"
#include "roms/rom82s123_7f.h"	// colours
#include "roms/rom82s126_4a.h"	// palette

void Display::begin() {
	UTFTDisplay::begin(VGA_BLACK, VGA_WHITE, portrait);
	clear();
	_xoff = (_dx - DISPLAY_WIDTH) / 2;
	_yoff = (_dy - DISPLAY_HEIGHT) / 2;
}

static void get_palette(palette_entry &p, byte index) {
	index <<= 2;
	p.set_colour(colours[palette[index  ]], 0);
	p.set_colour(colours[palette[index+1]], 1);
	p.set_colour(colours[palette[index+2]], 2);
	p.set_colour(colours[palette[index+3]], 3);
}

void Display::draw_tile(word t, int x, int y) {
	palette_entry p;
	get_palette(p, _tp[t + 0x0400]);

	byte tile = _tp[t];
	const byte *cdata = tiles + tile*64;
	for (int n = 0; n < 8; n++)
		for (int m = 0; m < 8; m++) {
			unsigned px = x+n, py = y+m;
			if (_dx > px && _dy > py) {
				colour &c = p.colours[*cdata];
				utft.setColor(c.red, c.green, c.blue);
				utft.drawPixel(px, py);
			}
			cdata++;
		}
}

void Display::_set(word t, byte b) {
	_tp[t] = b;
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
		word o = a - 0x40;
		x = 27 - o / 0x20;
		y = 2 + o % 0x20;
	} else if (a >= 0x3c2 && a < 0x3e0) {
		x = 27 - a+0x3c2;
		y = 0;
	} else if (a >= 0x3e2 && a < 0x400) {
		x = 27 - a+0x3e2;
		y = 1;
	}
	draw_tile(t, 8*x + _xoff, 8*y + _yoff);
}

void Display::set_sprite(word off, byte sx, byte sy) {
	int x = DISPLAY_WIDTH - sx + 15 + _xoff;
	int y = DISPLAY_HEIGHT - sy - 16 + _yoff;

	palette_entry p;
	get_palette(p, _mem[0x4ff1 + off]);

	byte sir = _mem[0x4ff0 + off];
	bool fx = (sir & 0x02), fy = (sir & 0x01);
	const byte *cdata = sprites + 256*(sir >> 2);
	for (int n = 0; n < 16; n++)
		for (int m = 0; m < 16; m++) {
			unsigned px = fx? x+15-n: x+n, py = fy? y+15-m: y+m;
			if (_dx > px && _dy > py) {
				colour &c = p.colours[*cdata];
				utft.setColor(c.red, c.green, c.blue);
				utft.drawPixel(px, py);
			}
			cdata++;
		}
}

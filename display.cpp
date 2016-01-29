#include <Energia.h>
#include <UTFT.h>
#include <r65emu.h>

#include "display.h"

#include "roms/rom5e.h"		// tiles
#include "roms/rom5f.h"		// sprites
#include "roms/rom82s123_7f.h"	// colours
#include "roms/rom82s126_4a.h"	// palette

void Display::begin() {
	UTFTDisplay::begin(VGA_BLACK, VGA_WHITE, portrait);
	clear();
	_xoff = (_dx - DISPLAY_WIDTH) / 2;
	_yoff = (_dy - DISPLAY_HEIGHT) / 2;
}

void Display::update() {
	draw_tiles();
}

static void get_tile_palette(palette_entry &p, byte index) {
	index <<= 2;
	p.set_colour(colours[palette[index  ]], 0);
	p.set_colour(colours[palette[index+1]], 1);
	p.set_colour(colours[palette[index+2]], 2);
	p.set_colour(colours[palette[index+3]], 3);
}

void Display::draw_sprite_slice(palette_entry &p, byte *cdata, int offset, int x, int y) {
	offset *= 32;
	for (int n = 7; n >= 0; n--)			// 8 columns
		for (int m = 3; m >= 0; m--) {		// 4 rows
			colour &c = p.colours[cdata[offset]];
			utft.setColor(c.red, c.green, c.blue);
// FIXME: investigate this
if (_dx <= (x+n)) {
continue;
}
if (_dy <= (y+m)) {
continue;
}

			utft.drawPixel(x+n, y+m);
			offset++;
		}
}

void Display::draw_tile(word a, int x, int y) {
	byte tile = _tiles[a];
	byte character[64];
	for (unsigned i = 0; i < 16; i++) {
		byte b = tiles[tile*16 + i];
		character[i*4  ] =  (b       & 0x01) | ((b >> 3) & 0x02);
		character[i*4+1] = ((b >> 1) & 0x01) | ((b >> 4) & 0x02);
		character[i*4+2] = ((b >> 2) & 0x01) | ((b >> 5) & 0x02);
		character[i*4+3] = ((b >> 3) & 0x01) | ((b >> 6) & 0x02);
	}

	palette_entry p;
	get_tile_palette(p, _tiles[a + 0x0400]);

	draw_sprite_slice(p, character, 0, x, y+4);
	draw_sprite_slice(p, character, 1, x, y);
}

void Display::_set(word a, byte b) {
	if (_tiles[a] != b) {
		_tiles[a] = b;
		int x, y;
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
		} else
			return;
		draw_tile(a, 8*x + _xoff, 8*y + _yoff);
	}
}

void Display::set_sprite(int n, byte sx, byte sy) {
	int x = DISPLAY_WIDTH - sx + 15 + _xoff;
	int y = DISPLAY_HEIGHT - sy - 16 + _yoff;
	byte sir = _mem[0x4ff0 + n*2], character[256];
	word si = 64*(sir >> 2);
	for (unsigned i = 0; i < 64; i++) {
		byte b = sprites[si + i];
		character[i*4  ] =  (b       & 0x01) | ((b >> 3) & 0x02);
		character[i*4+1] = ((b >> 1) & 0x01) | ((b >> 4) & 0x02);
		character[i*4+2] = ((b >> 2) & 0x01) | ((b >> 5) & 0x02);
		character[i*4+3] = ((b >> 3) & 0x01) | ((b >> 6) & 0x02);
	}

	palette_entry p;
	get_tile_palette(p, _mem[0x4ff1 + n*2]);

	draw_sprite_slice(p, character, 0, x+8, y+12);
	draw_sprite_slice(p, character, 1, x+8, y);
	draw_sprite_slice(p, character, 2, x+8, y+4);
	draw_sprite_slice(p, character, 3, x+8, y+8);

	draw_sprite_slice(p, character, 4, x, y+12);
	draw_sprite_slice(p, character, 5, x, y);
	draw_sprite_slice(p, character, 6, x, y+4);
	draw_sprite_slice(p, character, 7, x, y+8);

	// FIXME rotation
}

#pragma once

#define DISPLAY_WIDTH	224
#define DISPLAY_HEIGHT	288

#define TILE_COLS	28
#define TILE_ROWS	36
#define NUM_SPRITES	8

class colour {
public:
	uint8_t red, green, blue;

	void set(uint8_t r, uint8_t g, uint8_t b) {
		red = r; green = g; blue = b;
	}

	uint16_t get() {
		uint8_t fch = ((red & 248) | green >> 5);
		uint8_t fcl = ((green & 28)<<3 | blue >> 3);
		return (fch<<8) | fcl;
	}
};

class palette_entry {
public:
	colour colours[4];

	void set_colour(uint8_t c, int i) {
		uint8_t r = 0, g = 0, b = 0;
		if (c != 0) {
			r =  (c       & 0x01)*0x21 + ((c >> 1) & 0x01)*0x47 + ((c >> 2) & 0x01)*0x97;
			g = ((c >> 3) & 0x01)*0x21 + ((c >> 4) & 0x01)*0x47 + ((c >> 5) & 0x01)*0x97;
			b = ((c >> 6) & 0x01)*0x51 + ((c >> 7) & 0x01)*0xae;
		}
		colours[i].set(r, g, b);
	}
};

class Screen: public Display, public Memory::Device {
public:
	Screen(Memory &mem): Memory::Device(sizeof(_tp)), _mem(mem) {
		for (int i = 0; i < NUM_SPRITES; i++)
			_spr_x[i] = _spr_y[i] = -100;	// sentinel: nothing drawn there yet
		for (int y = 0; y < TILE_ROWS; y++)
			for (int x = 0; x < TILE_COLS; x++)
				_grid[y][x] = 0xffff;		// sentinel: no tile mapped to this cell yet
	}

	void operator=(uint8_t b) override { if (_tp[_acc] != b) _set(_acc, b); }
	operator uint8_t() override { return _tp[_acc]; }

	void checkpoint(Checkpoint &) override;
	void restore(Checkpoint &) override;
	void redraw();

	void begin();
	void set_sprite(uint16_t off, uint8_t sx, uint8_t sy);

private:
	void _set(uint16_t a, uint8_t b);
	void draw_tile(uint16_t addr, int x, int y);
	void _erase_sprite(int ox, int oy, int nx, int ny);

	uint8_t _tp[2048];
	uint16_t _palette565[32][4];
	uint16_t _grid[TILE_ROWS][TILE_COLS];		// tile-RAM address occupying each screen cell
	int16_t _spr_x[NUM_SPRITES], _spr_y[NUM_SPRITES];	// previous screen position per sprite slot

	Memory &_mem;
};

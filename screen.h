#ifndef __SCREEN_H__
#define __SCREEN_H__

#define DISPLAY_WIDTH	224
#define DISPLAY_HEIGHT	288

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
	Screen(Memory &mem): Memory::Device(sizeof(_tp)), _mem(mem) {}

	virtual void operator=(uint8_t b) { if (_tp[_acc] != b) _set(_acc, b); }
	virtual operator uint8_t() { return _tp[_acc]; }

	void begin();
	void set_sprite(uint16_t off, uint8_t sx, uint8_t sy);

private:
	void _set(uint16_t a, uint8_t b);
	void draw_tile(uint16_t addr, int x, int y);

	uint8_t _tp[2048];
	Memory &_mem;
};

#endif

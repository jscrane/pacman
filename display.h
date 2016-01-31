#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define DISPLAY_WIDTH	224
#define DISPLAY_HEIGHT	288

class colour {
public:
	byte red, green, blue;

	void set(byte r, byte g, byte b) {
		red = r; green = g; blue = b;
	}
};

class palette_entry {
public:
	colour colours[4];

	void set_colour(byte c, int i) {
		byte r = 0, g = 0, b = 0;
		if (c != 0) {
			r =  (c       & 0x01)*0x21 + ((c >> 1) & 0x01)*0x47 + ((c >> 2) & 0x01)*0x97;
			g = ((c >> 3) & 0x01)*0x21 + ((c >> 4) & 0x01)*0x47 + ((c >> 5) & 0x01)*0x97;
			b = ((c >> 6) & 0x01)*0x51 + ((c >> 7) & 0x01)*0xae;
		}
		colours[i].set(r, g, b);
	}
};

class Display: public UTFTDisplay, public Memory::Device {
public:
	Display(Memory &mem): Memory::Device(sizeof(_tiles)), _mem(mem) {}

	virtual void operator=(byte b) { _set(_acc, b); }
	virtual operator byte() { return _tiles[_acc]; }

	void begin();
	void set_sprite(word off, byte sx, byte sy);

private:
	void _set(word a, byte b);
	void draw_tile(word addr, int x, int y);

	unsigned _xoff, _yoff;
	byte _tiles[2048];
	Memory &_mem;
};

#endif

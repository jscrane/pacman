#ifndef __IO_H__
#define __IO_H__

#define INT_ENABLE	0x00
#define SOUND_ENABLE	0x01
#define FLIP_SCREEN	0x03
#define ONE_PLAYER_START	0x04
#define TWO_PLAYER_START	0x05
#define COIN_LOCKOUT	0x06
#define COIN_COUNTER	0x07

#define SOUND_START	0x40
#define SOUND_LEN	0x20
#define SPRITE_START	0x60
#define SPRITE_LEN	0x20
#define WATCHDOG	0xc0

#define INPUTS_0	0x00
#define INPUTS_1	0x40
#define DIP_1		0x80
#define DIP_2		0xc0

class IO: public Memory::Device {
public:
	IO(Display &display): Memory::Device(256), _display(display) {
		_up = _down = _left = _right = true;
		_coin = _p1_start = _p2_start = true;
	}

	void operator=(byte);
	operator byte();

	void down(byte key);
	void up(byte key);

	bool int_enabled() { return _int_enabled; }
	bool sound_enabled() { return _sound_enabled; }
	bool screen_flipped() { return _screen_flipped; }

	byte sprite_data(word addr) { return _sprites[addr - SPRITE_START]; }
	byte sound_data(word addr) { return _sound[addr - SOUND_START]; }

private:
	byte _sound[SOUND_LEN], _sprites[SPRITE_LEN];

	bool _up, _down, _left, _right, _coin, _p1_start, _p2_start;
	bool _int_enabled, _sound_enabled, _screen_flipped;

	Display &_display;
};

#endif

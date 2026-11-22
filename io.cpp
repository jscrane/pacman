#include <Arduino.h>
#include <r65emu.h>

#include "screen.h"
#include "switches.h"
#include "io.h"
#include "config.h"

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

void IO::operator=(uint8_t b) {
	if (_acc >= SPRITE_START && _acc < SPRITE_START + SPRITE_LEN) {
		if (_acc & 0x01)
			_screen.set_sprite(_acc-SPRITE_START-1, _sx, b);
		else
			_sx = b;
		return;
	}
	switch (_acc) {
	case INT_ENABLE:
		_int_enabled = (b & 0x01);
		break;
	case SOUND_ENABLE:
		_sound_enabled = (b & 0x01);
		break;
	case FLIP_SCREEN:
		_screen_flipped = (b & 0x01);
		break;
	}
}

IO::operator uint8_t() {
	if (_acc >= INPUTS_0 && _acc < INPUTS_1)
		return _switches.in0();

	if (_acc >= INPUTS_1 && _acc < DIP_1)
		return _switches.in1();

	if (_acc >= DIP_1)
		return DIP_SWITCHES;

	return 0;
}

void IO::checkpoint(Checkpoint &c) {
	c.write(_sx);
	c.write(_int_enabled);
	c.write(_sound_enabled);
	c.write(_screen_flipped);
	c.write(_vec);
}

void IO::restore(Checkpoint &c) {
	c.read(_sx);
	c.read(_int_enabled);
	c.read(_sound_enabled);
	c.read(_screen_flipped);
	c.read(_vec);
}

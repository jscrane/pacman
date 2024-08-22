#include <Arduino.h>
#include <r65emu.h>

#include "screen.h"
#include "io.h"
#include "config.h"

void IO::down(uint8_t key) {
	switch (key) {
	case KEY_UP:
		_up = false;
		break;
	case KEY_DOWN:
		_down = false;
		break;
	case KEY_LEFT:
		_left = false;
		break;
	case KEY_RIGHT:
		_right = false;
		break;
	case P1_START:
		_p1_start = false;
		break;
	case P2_START:
		_p2_start = false;
		break;
	case COIN:
		_coin = false;
		break;
	}
}

void IO::up(uint8_t key) {
	switch (key) {
	case KEY_UP:
		_up = true;
		break;
	case KEY_DOWN:
		_down = true;
		break;
	case KEY_LEFT:
		_left = true;
		break;
	case KEY_RIGHT:
		_right = true;
		break;
	case P1_START:
		_p1_start = true;
		break;
	case P2_START:
		_p2_start = true;
		break;
	case COIN:
		_coin = true;
		break;
	case PAUSE:
		_paused = !_paused;
		break;
	case ROTATE:
		_screen_flipped = !_screen_flipped;
		break;
	}
}

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
	if (_acc >= INPUTS_0 && _acc < INPUTS_1) {
		uint8_t v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_coin) v |= 0x20;
		return v;
	}
	if (_acc >= INPUTS_1 && _acc < DIP_1) {
		uint8_t v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_p1_start) v |= 0x20;
		if (_p2_start) v |= 0x40;
		return v;
	}
	if (_acc >= DIP_1)
		return NORMAL_NAMES | DIFFICULTY_NORMAL | BONUS_AT_20000 |
			ONE_LIFE_PER_GAME | ONE_COIN_ONE_GAME;
	return 0x00;
}

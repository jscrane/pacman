#include <Energia.h>
#include <PS2Keyboard.h>
#include <r65emu.h>

#include "display.h"
#include "io.h"
#include "config.h"

void IO::down(byte key) {
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

void IO::up(byte key) {
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
	case ROTATE:
		_screen_flipped = !_screen_flipped;
		break;
	}
}

void IO::operator=(byte b) {
	switch (_acc) {
	case INT_ENABLE:
//Serial.print("int enable ");
//Serial.println(b);
		_int_enabled = (b & 0x01);
		return;
	case SOUND_ENABLE:
//Serial.print("sound enable ");
//Serial.println(b);
		_sound_enabled = (b & 0x01);
		return;
	case FLIP_SCREEN:
//Serial.print("flip screen ");
//Serial.println(b);
		_screen_flipped = (b & 0x01);
		return;
	}
	if (_acc >= SOUND_START && _acc < SOUND_START + SOUND_LEN) {
		_sound[_acc - SOUND_START] = b;
		return;
	}
	if (_acc >= SPRITE_START && _acc < SPRITE_START + SPRITE_LEN) {
/*
Serial.print("write sprite ");
Serial.print(_acc, 16);
Serial.print(" ");
Serial.println(b, 16);
*/
		// FIXME: remove _sprites
		word off = _acc - SPRITE_START;
		_sprites[off] = b;
		if (off & 0x01)
			_display.set_sprite(off / 2, _sprites[off-1], _sprites[off]);
		return;
	}
	if (_acc >= WATCHDOG) {
//Serial.print("watchdog ");
//Serial.println(b, 16);
		return;
	}
//Serial.print("unknown io write ");
//Serial.println(_acc, 16);
}

IO::operator byte() {
	byte v = 0x00;
	if (_acc >= INPUTS_0 && _acc < INPUTS_1) {
		v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_coin) v |= 0x20;
//Serial.print("read inputs_0 ");
//Serial.println(v, 16);
		return v;
	}
	if (_acc >= INPUTS_1 && _acc < DIP_1) {
		v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_p1_start) v |= 0x20;
		if (_p2_start) v |= 0x40;
//Serial.print("read inputs_1 ");
//Serial.println(v, 16);
		return v;
	}
	if (_acc >= DIP_1 && _acc < DIP_2) {
//Serial.println("read dip_1");
		return 0xc9;
	}
	if (_acc >= DIP_2 && _acc < 0x100) {
//Serial.println("read dip_2");
		return 0xff;
	}
Serial.print("unknown io read ");
Serial.println(_acc, 16);
	return v;
}

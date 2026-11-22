#pragma once

#include <ps2_raw_kbd.h>
#include "switches.h"

#define P1_START	PS2_KEY_1
#define P2_START	PS2_KEY_2
#define COIN		PS2_KEY_3
#define PAUSE		PS2_SPACE

#define KBD_LEFT	PS2_KEYPAD_4
#define KBD_RIGHT	PS2_KEYPAD_6
#define KBD_UP		PS2_KEYPAD_8
#define KBD_DOWN	PS2_KEYPAD_2

class KbdSwitches: public Switches, public matrix_keyboard {
public:
	void down(uint16_t key) override;

	void up(uint16_t key) override;

	void reset() override { release_all(); }
};

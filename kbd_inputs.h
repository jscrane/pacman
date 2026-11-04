#pragma once

#include <ps2_raw_kbd.h>
#include "inputs.h"

#define P1_START	PS2_KEY_1
#define P2_START	PS2_KEY_2
#define COIN		PS2_KEY_3
#define PAUSE		PS2_SPACE

#define KEY_LEFT	PS2_KEYPAD_4
#define KEY_RIGHT	PS2_KEYPAD_6
#define KEY_UP		PS2_KEYPAD_8
#define KEY_DOWN	PS2_KEYPAD_2

class KbdInputs: public Inputs, public matrix_keyboard {
public:
	void down(uint16_t key) override;

	void up(uint16_t key) override;

	void reset() override { Inputs::reset(); }
};

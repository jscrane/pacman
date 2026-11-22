#include "kbd_switches.h"

void KbdSwitches::down(uint16_t scan) {
	switch (ps2_key(scan)) {
	case KBD_UP:
		_up = false;
		break;
	case KBD_DOWN:
		_down = false;
		break;
	case KBD_LEFT:
		_left = false;
		break;
	case KBD_RIGHT:
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

void KbdSwitches::up(uint16_t scan) {
	switch (ps2_key(scan)) {
	case KBD_UP:
		_up = true;
		break;
	case KBD_DOWN:
		_down = true;
		break;
	case KBD_LEFT:
		_left = true;
		break;
	case KBD_RIGHT:
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
	}
}

#pragma once

#include <cstdint>

class Switches {
public:
	uint8_t in0() const {
		uint8_t v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_coin) v |= 0x20;
		return v;
	}

	uint8_t in1() const {
		uint8_t v = 0x10;
		if (_up) v |= 0x01;
		if (_left) v |= 0x02;
		if (_right) v |= 0x04;
		if (_down) v |= 0x08;
		if (_p1_start) v |= 0x20;
		if (_p2_start) v |= 0x40;
		return v;
	}

	bool is_paused() const { return _paused; }

	void pause() { _paused = true; }

protected:
	Switches() { release_all(); }

	void release_all() {
		_up = _down = _left = _right = true;
		_coin = _p1_start = _p2_start = true;
		_paused = false;
	}

	bool _up, _down, _left, _right, _coin, _p1_start, _p2_start;
	bool _paused = false;
};

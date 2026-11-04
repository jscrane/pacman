#pragma once

class Screen;
class Inputs;

class IO: public Memory::Device {
public:
	IO(Screen &screen, Inputs &inputs): Memory::Device(256), _screen(screen), _inputs(inputs) {}

	void operator=(uint8_t) override;
	operator uint8_t() override;

	void checkpoint(Checkpoint &) override;
	void restore(Checkpoint &) override;

	bool int_enabled() const { return _int_enabled; }
	bool sound_enabled() const { return _sound_enabled; }
	bool screen_flipped() const { return _screen_flipped; }
	uint8_t vec() const { return _vec; }
	void vec(uint8_t b) { _vec = b; }

private:
	uint8_t _sx;
	uint8_t _vec;

	bool _int_enabled, _sound_enabled, _screen_flipped;

	Screen &_screen;
	Inputs &_inputs;
};

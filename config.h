#ifndef __CONFIG_H__
#define __CONFIG_H__

#define P1_START	0x16	// 1
#define P2_START	0x1e	// 2
#define COIN		0x26	// 3

#define KEY_LEFT	0x6b	// keypad #4
#define KEY_RIGHT	0x74	// keypad #6
#define KEY_UP		0x75	// keypad #8
#define KEY_DOWN	0x72	// keypad #2

#define PAUSE		0x29	// space
#define ROTATE		0x2d	// r

#if defined(USE_UTFT)
#define ORIENT		portrait
#elif defined(USE_ESPI)
#define ORIENT		reverse_portrait
#elif defined(USE_VGA)
#define ORIENT		landscape
#endif

#endif

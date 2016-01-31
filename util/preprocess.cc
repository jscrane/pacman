#include <stdio.h>
#include <stdint.h>

#include <rom5e.h>	// tiles
#include <rom5f.h>	// sprites

int main(int argc, char *argv[]) {

	printf("static const unsigned char tiles[] = {");
	for (int t = 0; t < 256; t++) {
		uint8_t c[64];
		for (int i = 0; i < 16; i++) {
                	uint8_t b = tiles[t*16 + i];
                	c[i*4  ] =  (b       & 0x01) | ((b >> 3) & 0x02);
                	c[i*4+1] = ((b >> 1) & 0x01) | ((b >> 4) & 0x02);
                	c[i*4+2] = ((b >> 2) & 0x01) | ((b >> 5) & 0x02);
                	c[i*4+3] = ((b >> 3) & 0x01) | ((b >> 6) & 0x02);
		}
		for (int i = 0; i < sizeof(c); i++) {
			if ((i % 8) == 0)
				printf("\n\t");
			printf("0x%02x, ", c[i]);
		}
	}
	printf("\n};\n");

	printf("static const unsigned char sprites[] = {");
	for (int s = 0; s < 64; s++) {
		uint8_t c[256];
		for (int i = 0; i < 64; i++) {
                	uint8_t b = sprites[s*64 + i];
                	c[i*4  ] =  (b       & 0x01) | ((b >> 3) & 0x02);
                	c[i*4+1] = ((b >> 1) & 0x01) | ((b >> 4) & 0x02);
                	c[i*4+2] = ((b >> 2) & 0x01) | ((b >> 5) & 0x02);
                	c[i*4+3] = ((b >> 3) & 0x01) | ((b >> 6) & 0x02);
		}
		for (int i = 0; i < sizeof(c); i++) {
			if ((i % 8) == 0)
				printf("\n\t");
			printf("0x%02x, ", c[i]);
		}
	}
	printf("\n};\n");
}

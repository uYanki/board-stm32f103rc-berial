#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

//Usage:
//ffmpeg -i "*FileIn*" -vf colorchannelmixer=.299:.587:.114:0:.299:.587:.114:0:.299:.587:.114,scale=23:17:flags=lanczos -pix_fmt gray -f rawvideo - | pic-encoder

#define mapdata(data, to) (((data) > 127 ? 1 : 0) << (to))
#define index(dx, dy) ((23 * (y * 9 + (dy))) + (x * 6 + (dx)))

void encode(uint8_t framedata[23 * 17], uint8_t out[5 * 8]) {
	memset(out, 0, 5 * 8);
	for (size_t y = 0; y < 2; y++) {
		for (size_t x = 0; x < 4; x++) {
			size_t i = 5 * (y * 4 + x);
			out[i + 0] =
				mapdata(framedata[index(4, 0)], 0) |
				mapdata(framedata[index(3, 0)], 1) |
				mapdata(framedata[index(2, 0)], 2) |
				mapdata(framedata[index(1, 0)], 3) |
				mapdata(framedata[index(0, 0)], 4) |
				mapdata(framedata[index(4, 1)], 5) |
				mapdata(framedata[index(3, 1)], 6) |
				mapdata(framedata[index(2, 1)], 7);
			out[i + 1] =
				mapdata(framedata[index(1, 1)], 0) |
				mapdata(framedata[index(0, 1)], 1) |
				mapdata(framedata[index(4, 2)], 2) |
				mapdata(framedata[index(3, 2)], 3) |
				mapdata(framedata[index(2, 2)], 4) |
				mapdata(framedata[index(1, 2)], 5) |
				mapdata(framedata[index(0, 2)], 6) |
				mapdata(framedata[index(4, 3)], 7);
			out[i + 2] =
				mapdata(framedata[index(3, 3)], 0) |
				mapdata(framedata[index(2, 3)], 1) |
				mapdata(framedata[index(1, 3)], 2) |
				mapdata(framedata[index(0, 3)], 3) |
				mapdata(framedata[index(4, 4)], 4) |
				mapdata(framedata[index(3, 4)], 5) |
				mapdata(framedata[index(2, 4)], 6) |
				mapdata(framedata[index(1, 4)], 7);
			out[i + 3] =
				mapdata(framedata[index(0, 4)], 0) |
				mapdata(framedata[index(4, 5)], 1) |
				mapdata(framedata[index(3, 5)], 2) |
				mapdata(framedata[index(2, 5)], 3) |
				mapdata(framedata[index(1, 5)], 4) |
				mapdata(framedata[index(0, 5)], 5) |
				mapdata(framedata[index(4, 6)], 6) |
				mapdata(framedata[index(3, 6)], 7);
			out[i + 4] =
				mapdata(framedata[index(2, 6)], 0) |
				mapdata(framedata[index(1, 6)], 1) |
				mapdata(framedata[index(0, 6)], 2) |
				mapdata(framedata[index(4, 7)], 3) |
				mapdata(framedata[index(3, 7)], 4) |
				mapdata(framedata[index(2, 7)], 5) |
				mapdata(framedata[index(1, 7)], 6) |
				mapdata(framedata[index(0, 7)], 7);
		}
	}
}

int main() {
#ifdef _WIN32
	_setmode(_fileno(stdin), _O_BINARY);
#endif
	FILE *outfile = fopen("out.bin", "wb");
	uint8_t buffer[23 * 17];
	uint8_t out[5 * 8];
	size_t i = 0;
	for (;;) {
		size_t ret = 0;
		printf("#%04zd\n", ++i);
		ret = fread(buffer, 23 * 17, 1, stdin);
		if (ret < 1) {
			printf("EOF!\n");
			break;
		}
		encode(buffer, out);
		fwrite(out, 5 * 8, 1, outfile);
	}
	fclose(outfile);
	return 0;
}


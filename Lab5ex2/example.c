#include "lodepng.h"

#include <stdio.h>
#include <stdlib.h>
extern void to_gray_asm(unsigned char * image, int width, int height);
void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
	/*Encode the image*/
	unsigned error = lodepng_encode32_file(filename, image, width, height);

	/*if there's an error, display it*/
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));
}
void encodeTwoSteps(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
	unsigned char* png;
	size_t pngsize;

	unsigned error = lodepng_encode32(&png, &pngsize, image, width, height);
	if (!error) lodepng_save_file(png, pngsize, filename);

	/*if there's an error, display it*/
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

	free(png);
}
void encodeWithState(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
	unsigned error;
	unsigned char* png;
	size_t pngsize;
	LodePNGState state;

	lodepng_state_init(&state);
	/*optionally customize the state*/

	error = lodepng_encode(&png, &pngsize, image, width, height, &state);
	if (!error) lodepng_save_file(png, pngsize, filename);

	/*if there's an error, display it*/
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

	lodepng_state_cleanup(&state);
	free(png);
}

void to_gray(unsigned char * image, int width, int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned r = image[4 * width * y + 4 * x + 0];
			unsigned g = image[4 * width * y + 4 * x + 1];
			unsigned b = image[4 * width * y + 4 * x + 2];
			unsigned gray = (r + g + b) / 3;
			image[4 * width * y + 4 * x + 0] =
				image[4 * width * y + 4 * x + 1] =
				image[4 * width * y + 4 * x + 2] = gray;
		}
	}
}
void to_planar(unsigned char * inp, unsigned char * out,
	int width, int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int c = 0; c < 4; c++)
			{
				out[c * (width * height) + width * y + x] =
					inp[4 * width * y + 4 * x + c];
			}
		}
	}
}
void to_rgba(unsigned char * inp, unsigned char * out,
	int width, int height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int c = 0; c < 4; c++)
			{
				out[4 * width * y + 4 * x + c] =
					inp[c * (width * height) + width * y + x];
			}
		}
	}
}

int main(int argc, char *argv[])
{
	const char* filename = argc > 1 ? argv[1] : "test.png";

	/*generate some image*/
	unsigned width = 512, height = 512;
	unsigned char* image = malloc(width * height * 4);
	unsigned char* planar_img = malloc(width * height * 4);
	unsigned char* rgba_img = malloc(width * height * 4);
	unsigned x, y;
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
		{
			image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
			image[4 * width * y + 4 * x + 1] = x ^ y;
			image[4 * width * y + 4 * x + 2] = x | y;
			image[4 * width * y + 4 * x + 3] = 255;
		}

	/*run an example*/
	to_planar(image, planar_img, width, height);
	//to_gray(planar_img, width, height);
	to_gray_asm(planar_img, width, height);
	to_rgba(planar_img, rgba_img, width, height);
	encodeOneStep(filename, rgba_img, width, height);


	free(image);
	return 0;
}

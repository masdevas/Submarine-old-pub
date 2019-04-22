#ifndef PIXEL_MOBSEL
#define PIXEL_MOBSEL

#include <gd.h>
#include <stdlib.h>
#include "settings.h"

#define INCORRECT_X -1
#define INCORRECT_Y -1

typedef struct PixelEth
{
	//Color
	int color;

	//Direct
	byte direct;

	//Object
	int object;
} PixelEth;

typedef struct Pixel
{
	//Coordinates
	coord_t x;
	coord_t y;

	//Color
	int color;

	//Direct
	byte direct;

	//Object
	int object;

	//Flags
	byte flagComb;
	byte flagAttach;

} Pixel;

Pixel** createMapPixels(coord_t sx, coord_t sy);
PixelEth** createMapPixelsEth(coord_t sx, coord_t sy);
void initMapByNulls(gdImagePtr im, Pixel **mapPixels, coord_t sx, coord_t sy);
void formatMap(Pixel **mapPixel, PixelEth **mapPixelEth, coord_t sx, coord_t sy);

#endif
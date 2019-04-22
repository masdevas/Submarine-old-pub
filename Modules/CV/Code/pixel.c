#include "Headers/pixel.h"

Pixel** createMapPixels(coord_t sx, coord_t sy)
{
	coord_t i;
	Pixel **mapPixels;

	mapPixels = (Pixel**)malloc(sizeof(Pixel*) * sx);
	for(i = 0; i < sx; i++)
		mapPixels[i] = (Pixel*)malloc(sizeof(Pixel) * sy);

	return mapPixels;
}

PixelEth** createMapPixelsEth(coord_t sx, coord_t sy)
{
	coord_t i;
	PixelEth **mapPixelsEth;

	mapPixelsEth = (PixelEth**)malloc(sizeof(PixelEth*) * sx);
	for(i = 0; i < sx; i++)
		mapPixelsEth[i] = (PixelEth*)malloc(sizeof(PixelEth) * sy);

	return mapPixelsEth;
}

void initMapByNulls(gdImagePtr im, Pixel **mapPixels, coord_t sx, coord_t sy)
{
	coord_t i, j;
	
	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
		{
			mapPixels[i][j].x = i;
			mapPixels[i][j].y = j;
			mapPixels[i][j].color = gdImageGetPixel(im, i, j);
			mapPixels[i][j].direct = 0;
			mapPixels[i][j].object = IS_UNDEFINED;
			mapPixels[i][j].flagComb = 0;
			mapPixels[i][j].flagAttach = 0;
		}
}

void formatMap(Pixel **mapPixel, PixelEth **mapPixelEth, coord_t sx, coord_t sy)
{
	coord_t i, j;

	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
		{
			mapPixelEth[i][j].color = mapPixel[i][j].color;
			mapPixelEth[i][j].direct = mapPixel[i][j].direct;
			mapPixelEth[i][j].object = mapPixel[i][j].object;
		}
}
#ifndef FILL_DATA_MOBSEL
#define FILL_DATA_MOBSEL

#include "pixel.h"
#include "objects_params.h"
#include "queue_pixel.h"
#include "equivalence_tree.h"

typedef struct CVData
{
	coord_t sx;
	coord_t sy;
	int countObjects;
	off_t sizeImg;
	PixelEth **map;
	Pixel **mapPixels;
	ObjParams *objects;
	QueuePixel *queuePix;
	EqTree *eqTree;
} CVData;

#endif

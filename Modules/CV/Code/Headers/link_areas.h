#ifndef LINK_AREAS_MOBSEL
#define LINK_AREAS_MOBSEL

#include <stdlib.h>
#include <gd.h>
#include "pixel.h"
#include "settings.h"
#include "objects_params.h"
#include "queue_pixel.h"
#include "equivalence_tree.h"

#ifdef DEBUG
#include <stdio.h>
gdImagePtr resImage;
#endif

/* Selection objects */
void linkAreas(gdImagePtr im, Pixel **mapPixels, coord_t sx, coord_t sy, int *indexLastObj, EqTree *eqTree, QueuePixel *queuePix);
void searchObjects(Pixel **mapPixels, coord_t sx, coord_t sy, int *indexLastObj, EqTree *eqTree, QueuePixel *queuePix);
void linkObject(Pixel **mapPixels, coord_t sx, coord_t sy, coord_t x, coord_t y, int *indexLastObj, EqTree *eqTree, QueuePixel *queuePix);

/* Count difference between two pixels */
void checkVertWay(gdImagePtr im, Pixel **mapPixels, coord_t i, coord_t j);
void checkHorizWay(gdImagePtr im, Pixel **mapPixels, coord_t i, coord_t j);
int getDiff(gdImagePtr im, int pixel1, int pixel2);

#endif

#ifndef FIND_REAL_OBJ_MOBSEL
#define FIND_REAL_OBJ_MOBSEL

#include "settings.h"
#include "pixel.h"
#include "queue_pixel.h"
#include "objects_params.h"
#include "equivalence_tree.h"

void findRealObjects(Pixel **mapPixels, coord_t sx, coord_t sy, EqTree *eqTree, ObjParams *realObjParams, int *indexLastRealObj, QueuePixel *queuePix);
int combineWithNearbyPseudo(Pixel **mapPixels, coord_t sx, coord_t sy, coord_t x, coord_t y, EqTree *eqTree, QueuePixel *queuePix);
void attachToNearbyReal(Pixel **mapPixels, coord_t sx, coord_t sy, coord_t x, coord_t y, EqTree *eqTree, QueuePixel *queuePix, ObjParams *realObjParams, int sizeCurObj);
void pushAndPullRealObject(EqTree *eqTree, int indexLastObj);

#endif
#ifndef OBJPARAMS_MOBSEL
#define OBJPARAMS_MOBSEL

#include "pixel.h"
#include "equivalence_tree.h"

typedef struct ObjParams
{
	coord_t x;
	coord_t y;
	int size;
	int perimeter;
	coord_t maxX;
	coord_t minX;
	coord_t maxY;
	coord_t minY;
	//int objectOnMap;
} ObjParams;

void initRealObjParams(ObjParams *realObjParams, EqTree *eqTree, int indexLastObj, int *indexLastRealObj);
void repairRealObjects(Pixel **map, EqTree *eqTree, coord_t sx, coord_t sy);
void pushAndPullRealObject(EqTree *eqTree, int indexLastObj);
void countCharacters(Pixel **map, ObjParams *realObjParams, coord_t sx, coord_t sy);

#endif
#include "Headers/objects_params.h"

void initRealObjParams(ObjParams *realObjParams, EqTree *eqTree, int indexLastObj, int *indexLastRealObj)
{
	int i;

	for(i = 0; i <= indexLastObj; i++)
	{
		if(eqTree->vertexes[i].size >= MIN_SIZE)
		{
			(*indexLastRealObj)++;
			realObjParams[*indexLastRealObj].x = eqTree->vertexes[i].x;
			realObjParams[*indexLastRealObj].y = eqTree->vertexes[i].y;
			realObjParams[*indexLastRealObj].maxX = COORD_MIN;
			realObjParams[*indexLastRealObj].minX = COORD_MAX;
			realObjParams[*indexLastRealObj].maxY = COORD_MIN;
			realObjParams[*indexLastRealObj].minY = COORD_MAX;
			realObjParams[*indexLastRealObj].size = eqTree->vertexes[i].size;
			realObjParams[*indexLastRealObj].perimeter = 0;
			eqTree->vertexes[i].realObject = *indexLastRealObj;
		}
	}
}

void repairRealObjects(Pixel **map, EqTree *eqTree, coord_t sx, coord_t sy)
{
	coord_t i, j;
	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
			map[i][j].object = eqTree->vertexes[map[i][j].object].realObject;
}


void pushAndPullRealObject(EqTree *eqTree, int indexLastObj)
{
	int i;

	for(i = 0; i < indexLastObj; i++)
		if(eqTree->vertexes[i].realObject != IS_UNDEFINED)
			eqtreePushRealObject(eqTree, i);
	eqtreePullRealObjects(eqTree);
}

void countCharacters(Pixel **map, ObjParams *realObjParams, coord_t sx, coord_t sy)
{
	coord_t i, j;

	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
		{
			/* Perimeters */
			if(i == 0 || i == sx - 1 || j == 0 || j == sy - 1)
			{
				realObjParams[map[i][j].object].perimeter++;
			}else if(map[i][j].object != map[i + 1][j].object)
			{
				realObjParams[map[i][j].object].perimeter++;
			}else if(map[i][j].object != map[i - 1][j].object)
			{
				realObjParams[map[i][j].object].perimeter++;
			}else if(map[i][j].object != map[i][j - 1].object)
			{
				realObjParams[map[i][j].object].perimeter++;
			}else if(map[i][j].object != map[i][j + 1].object)
			{
				realObjParams[map[i][j].object].perimeter++;
			}

			/* Max coordinates (may be translate to linkAreas and another) */
			if(i > realObjParams[map[i][j].object].maxX)
				realObjParams[map[i][j].object].maxX = i;
			if(j > realObjParams[map[i][j].object].maxY)
				realObjParams[map[i][j].object].maxY = j;
			if(i < realObjParams[map[i][j].object].minX)
				realObjParams[map[i][j].object].minX = i;
			if(j < realObjParams[map[i][j].object].minY)
				realObjParams[map[i][j].object].minY = j;

		}
}
#include "Headers/find_real_objects.h"

void findRealObjects(Pixel **mapPixels, coord_t sx, coord_t sy, EqTree *eqTree, ObjParams *realObjParams, int *indexLastRealObj, QueuePixel *queuePix)
{
	int i, j, sizeCurObj = 0;

	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
		{
			if(eqTree->vertexes[mapPixels[i][j].object].size < MIN_SIZE)
			{
				sizeCurObj = combineWithNearbyPseudo(mapPixels, sx, sy, i, j, eqTree, queuePix);
				if(sizeCurObj <= 0)
					continue;
				if(sizeCurObj < MIN_SIZE)
				{
					attachToNearbyReal(mapPixels, sx, sy, i, j, eqTree, queuePix, realObjParams, sizeCurObj);
					continue;
				}else
				{
					(*indexLastRealObj)++;
					realObjParams[*indexLastRealObj].size = sizeCurObj;
					realObjParams[*indexLastRealObj].x = i;
					realObjParams[*indexLastRealObj].y = j;
					eqTree->vertexes[mapPixels[i][j].object].realObject = *indexLastRealObj;
				}
			}
		}
}

int combineWithNearbyPseudo(Pixel **mapPixels, coord_t sx, coord_t sy, coord_t x, coord_t y, EqTree *eqTree, QueuePixel *queuePix)
{
	int curSize = eqTree->vertexes[mapPixels[x][y].object].size, curObj = mapPixels[x][y].object;
	Pixel curPix;
	if(mapPixels[x][y].flagComb == 1)
		return -1;
	qpixEnqueue(queuePix, mapPixels[x][y]);
	while(qpixIsNotEmpty(queuePix))
	{
		curPix = qpixDequeue(queuePix);
		if(mapPixels[curPix.x][curPix.y].flagComb == 0)
			mapPixels[curPix.x][curPix.y].flagComb = 1;
		else
			continue;

		//If object is Pseudo
		if(eqTree->vertexes[mapPixels[curPix.x][curPix.y].object].realObject == IS_UNDEFINED)
		{
			if(mapPixels[curPix.x][curPix.y].object != curObj)
			{
				eqtreeLinkVertexes(eqTree, mapPixels[curPix.x][curPix.y].object, curObj);
				curSize++;
			}

			//UP_WAY
			if(curPix.y < sy - 1)

				if(eqTree->vertexes[mapPixels[curPix.x][curPix.y + 1].object].realObject == IS_UNDEFINED)
					qpixEnqueue(queuePix, mapPixels[curPix.x][curPix.y + 1]);

			//RIGHT_WAY
			if(curPix.x < sx - 1)
				if(eqTree->vertexes[mapPixels[curPix.x + 1][curPix.y].object].realObject == IS_UNDEFINED)
					qpixEnqueue(queuePix, mapPixels[curPix.x + 1][curPix.y]);

			//DOWN_WAY
			if(curPix.y > 0)
				if(eqTree->vertexes[mapPixels[curPix.x][curPix.y - 1].object].realObject == IS_UNDEFINED)
					qpixEnqueue(queuePix, mapPixels[curPix.x][curPix.y - 1]);

			//LEFT_WAY
			if(curPix.x > 0)
				if(eqTree->vertexes[mapPixels[curPix.x - 1][curPix.y].object].realObject == IS_UNDEFINED)
					qpixEnqueue(queuePix, mapPixels[curPix.x - 1][curPix.y]);
		}
	}
	return curSize;
}

//TODO member nearby real object early, in the combineWithNearbyPseudo
void attachToNearbyReal(Pixel **mapPixels, coord_t sx, coord_t sy, coord_t x, coord_t y, EqTree *eqTree, QueuePixel *queuePix, ObjParams *realObjParams, int sizeCurObj)
{
	int curObj = mapPixels[x][y].object;
	Pixel curPix;
	if(mapPixels[x][y].flagAttach == 1)
		return;
	qpixEnqueue(queuePix, mapPixels[x][y]);
	while(qpixIsNotEmpty(queuePix))
	{
		curPix = qpixDequeue(queuePix);
		if(mapPixels[curPix.x][curPix.y].flagAttach == 0)
			mapPixels[curPix.x][curPix.y].flagAttach = 1;
		else
			continue;
		if(eqTree->vertexes[mapPixels[curPix.x][curPix.y].object].realObject != IS_UNDEFINED)
		{	
			eqtreeLinkVertexes(eqTree, mapPixels[curPix.x][curPix.y].object, curObj);
			realObjParams[eqTree->vertexes[mapPixels[curPix.x][curPix.y].object].realObject].size += sizeCurObj;
			return;
		}

		//UP_WAY
		if(curPix.y > 0)
			qpixEnqueue(queuePix, mapPixels[curPix.x][curPix.y - 1]);

		//RIGHT_WAY
		if(curPix.x < sx - 1)
			qpixEnqueue(queuePix, mapPixels[curPix.x + 1][curPix.y]);

		//DOWN_WAY
		if(curPix.y < sy - 1)
			qpixEnqueue(queuePix, mapPixels[curPix.x][curPix.y + 1]);

		//LEFT_WAY
		if(curPix.x > 0)
			qpixEnqueue(queuePix, mapPixels[curPix.x - 1][curPix.y]);

	}
}

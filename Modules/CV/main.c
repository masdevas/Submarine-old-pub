#include <gd.h>
#include <stdlib.h>
#include "Code/Headers/settings.h"
#include "Code/Headers/pixel.h"
#include "Code/Headers/objects_params.h"
#include "Code/Headers/link_areas.h"
#include "Code/Headers/queue_pixel.h"
#include "Code/Headers/equivalence_tree.h"
#include "Code/Headers/find_real_objects.h"

gdImagePtr resImage;

#ifdef SHOW_CPU_TIME
double getCPUTime(void);
#endif

#ifdef DEBUG
void drawObjects(Pixel **mapPixels, int *colors, EqTree *eqTree, coord_t sx, coord_t sy, int indexLastRealObj);
#endif

int main(void)
{
	/* Declaration */
	int i, indexLastObj = -1, indexLastRealObj = -1, *colors;
	coord_t sx, sy;
	double startTime, endTime;			
	gdImagePtr im;
	FILE *pngIn, *pngOut;
	Pixel **mapPixels;
	PixelEth **mapPixelsEth;
	ObjParams *realObjParams;
	EqTree *eqTree;
	QueuePixel *queuePix;
	

	/* Start working */
	#ifdef SHOW_CPU_TIME
	startTime = getCPUTime();
	#endif


	/* Data preparation */
	//Prepare files
	pngIn = fopen("Data/katya.png", "rb");
	im = gdImageCreateFromPng(pngIn);
	sx = (coord_t)gdImageSX(im);		//Can be hardcoded
	sy = (coord_t)gdImageSY(im);		//Can be hardcoded

	#ifdef DEBUG
	pngOut = fopen("Results/katya_res.png", "wb");
	resImage = gdImageCreate((int)sx, (int)sy);
	printf("Sx %hu, Sy %hu\n", sx, sy);
	#endif

	//Prepare arrays
	mapPixels = createMapPixels(sx, sy);
	mapPixelsEth = createMapPixelsEth(sx, sy);
	eqTree = eqtreeInitEqTree(sx * sy);
	queuePix = qpixInitQueuePixel(sx * sy);
	initMapByNulls(im, mapPixels, sx, sy);		//May be delete, if map contains nulls by default

	/* Data processing */ 
	//gdImageSmooth(im, 0.5);		//~0.4sec

	linkAreas(im, mapPixels, sx, sy, &indexLastObj, eqTree, queuePix);
	realObjParams = (ObjParams*)malloc(sizeof(ObjParams) * (indexLastObj + 1));
	initRealObjParams(realObjParams, eqTree, indexLastObj, &indexLastRealObj);
	findRealObjects(mapPixels, sx, sy, eqTree, realObjParams, &indexLastRealObj, queuePix);
	pushAndPullRealObject(eqTree, indexLastObj);
	repairRealObjects(mapPixels, eqTree, sx, sy);
	countCharacters(mapPixels, realObjParams, sx, sy);
	formatMap(mapPixels, mapPixelsEth, sx, sy);

	#ifdef DEBUG

	/*printf("Pseudo objects %i\n", (indexLastObj + 1));
	for(i = 0; i <= indexLastObj; i++)
		printf("Obj[%i].size:%i\n", i,  eqTree->vertexes[i].size);*/

	printf("Real objects %i\n", (indexLastRealObj + 1));
	colors = (int*)malloc(sizeof(int) * (indexLastRealObj + 1));
	for(i = 0; i <= indexLastRealObj; i++)
	{
		printf("Params %i : x %hu, y %hu, size %i, perim %i, maxX %i, minX %i, maxY %i, minY %i\n",
		 i, realObjParams[i].x, realObjParams[i].y, realObjParams[i].size, realObjParams[i].perimeter,
		 realObjParams[i].maxX, realObjParams[i].minX, realObjParams[i].maxY, realObjParams[i].minY);
	}
	for(i = 0; i <= indexLastRealObj; i++)
		colors[i] = gdImageColorAllocate(resImage, (i * 13) % 255, 255 - (i * 23) % 255, 255 + (49 * i) % 255);
	drawObjects(mapPixels, colors, eqTree, sx, sy, indexLastRealObj);
	#endif
  	/* Closing files */
	gdImageDestroy(im);
	fclose(pngIn);
	
	#ifdef DEBUG
	gdImagePng(resImage, pngOut);
	gdImageDestroy(resImage);
	fclose(pngOut);
	#endif

	/* End working*/
	//ADD ALL FREE

	eqtreeClearLink(eqTree);
	#ifdef DEBUG
	free(colors);
	#endif

	#ifdef SHOW_CPU_TIME
	endTime = getCPUTime();
	fprintf(stdout, "CPU time used = %lf\n", (endTime - startTime));	
	#endif

	return 0;
}

#ifdef DEBUG

void drawObjects(Pixel **mapPixels, int *colors, EqTree *eqTree, coord_t sx, coord_t sy, int indexLastRealObj)
{
	coord_t i, j;
	int isUndef = gdImageColorAllocate(resImage, 0, 0, 0);

	//printf("000\n");
	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
		{

			//printf("111\n");
			if(eqTree->vertexes[mapPixels[i][j].object].realObject == IS_UNDEFINED)
			{
				//if(mapPixels[i][j].object == IS_UNDEFINED)
				//	printf("WWW\n");
				gdImageLine(resImage, i, j, i, j, isUndef);
				//printf("333\n");
			}
			else
			{
				//printf("444 %i %i\n", mapPixels[i][j].object, indexLastRealObj);
				gdImageLine(resImage, i, j, i, j, mapPixels[i][j].object);
			}
			//printf("555\n");
		}
	//printf("222\n");
}

#endif
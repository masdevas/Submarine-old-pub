#include <gd.h>
#include <stdlib.h>
#include "Headers/settings.h"
#include "Headers/pixel.h"
#include "Headers/objects_params.h"
#include "Headers/data_prepare.h"
#include "Headers/link_areas.h"
#include "Headers/queue_pixel.h"
#include "Headers/equivalence_tree.h"
#include "Headers/find_real_objects.h"

extern gdImagePtr resImage;

//TODO Add vertexes and generate colors only for objects with size more than MIN_SIZE

//FOR DEBUG
double getCPUTime(void);

#ifdef DEBUG
void drawObjects(Pixel **mapPixels, int *colors, EqTree *eqTree, int sx, int sy);
#endif

int main(void)
{
	/* Declaration */
	int i, sx, sy, indexLastObj = -1, indexLastRealObj = -1, *colors;
	double startTime, endTime;			
	gdImagePtr im;
	FILE *pngIn, *pngOut;
	Pixel **mapPixels;
	ObjParams *realObjParams;
	EqTree *eqTree;
	QueuePixel *queuePix;
	

	/* Start working */
	#ifdef DEBUG
	startTime = getCPUTime();
	#endif


	/* Data preparation */
	//Prepare files
	pngIn = fopen("Data/real_tst2.jpg", "rb");
	im = gdImageCreateFromJpeg(pngIn);
	sx = gdImageSX(im);		//Can be hardcoded
	sy = gdImageSY(im);		//Can be hardcoded

	#ifdef DEBUG
	pngOut = fopen("Results/real_tst2_res.png", "wb");
	resImage = gdImageCreate(sx, sy);
	printf("Sx %i, Sy %i\n", sx, sy);
	#endif

	//Prepare arrays
	mapPixels = (Pixel**)malloc(sizeof(Pixel*) * sx);
	for(i = 0; i < sx; i++)
		mapPixels[i] = (Pixel*)malloc(sizeof(Pixel) * sy);
	eqTree = eqtreeInitEqTree(sx * sy);
	queuePix = qpixInitQueuePixel(sx * sy);
	initMapByNulls(im, mapPixels, sx, sy);		//May be delete, if map contains nulls by default

	
	/* Data processing */ 
	//gdImageSmooth(im, 0.5);		//~0.4sec

	linkAreas(im, mapPixels, sx, sy, &indexLastObj, eqTree, queuePix);
	realObjParams = (ObjParams*)malloc(sizeof(ObjParams) * indexLastObj);
	initRealObjParams(realObjParams, eqTree, indexLastObj, &indexLastRealObj);
	//findRealObjects(mapPixels, sx, sy, eqTree, realObjParams, &indexLastRealObj, queuePix);
	//pushAndPullRealObject(eqTree, indexLastObj);

	#ifdef DEBUG
	/*printf("Pseudo objects %i\n", (indexLastObj + 1));
	for(i = 0; i <= indexLastObj; i++)
		printf("Obj[%i].size:%i\n", i,  eqTree->vertexes[i].size);*/

	printf("Real objects %i\n", (indexLastRealObj + 1));
	colors = (int*)malloc(sizeof(int) * (indexLastRealObj + 1));
	for(i = 0; i <= indexLastRealObj; i++)
		colors[i] = gdImageColorAllocate(resImage, (i * 13) % 255, 255 - (i * 23) % 255, 255 + (49 * i) % 255);
	drawObjects(mapPixels, colors, eqTree, sx, sy);
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
	#ifdef DEBUG
	free(colors);
	endTime = getCPUTime();
	fprintf(stderr, "CPU time used = %lf\n", (endTime - startTime));	
	#endif

	return 0;
}

#ifdef DEBUG

void drawObjects(Pixel **mapPixels, int *colors, EqTree *eqTree, int sx, int sy)
{
	int i, j, isUndef = gdImageColorAllocate(resImage, 0, 0, 0);

	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
			if(eqTree->vertexes[mapPixels[i][j].object].realObject == IS_UNDEFINED)
			{
				//printf("WWW\n");
				gdImageLine(resImage, i, j, i, j, isUndef);
			}
			else
				gdImageLine(resImage, i, j, i, j, colors[ eqTree->vertexes[mapPixels[i][j].object].realObject ]);
}

#endif
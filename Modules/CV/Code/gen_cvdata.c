#include "Headers/gen_cvdata.h"

CVData* genCVData(char *phName, CVData *data)	//Why not generateCVData???
{
	/* Declaration */
	int indexLastObj = -1, indexLastRealObj = -1;
	#ifndef MAKE_GLOBAL_SXSY
	coord_t sx, sy;
	#endif			
	gdImagePtr im;
	FILE *pngIn;
	struct stat st;

	/* Data preparation */
	//Prepare photo params
	pngIn = fopen(phName, "rb");
	im = gdImageCreateFromPng(pngIn);
	stat(phName, &st);
	data->sizeImg = st.st_size;
	#ifndef MAKE_GLOBAL_SXSY
	data->sx = (coord_t)gdImageSX(im);		//Can be hardcoded
	data->sy = (coord_t)gdImageSY(im);		//Can be hardcoded
	#else
	data->sx = sx;
	data->sy = sy;
	#endif
	if(flagInit == VARS_NOT_INITIALIZED)
	{
		initDataForCV(data, data->sx, data->sy, st.st_size);
		flagInit = VARS_INITIALIZED;
	}

	initMapByNulls(im, data->mapPixels, data->sx, data->sy);		//May be delete, if map contains nulls by default
	/* Data processing */
	//gdImageSmooth(im, 0.5);		//~0.4sec
	linkAreas(im, data->mapPixels, data->sx, data->sy, &indexLastObj, data->eqTree, data->queuePix);
	initRealObjParams(data->objects, data->eqTree, indexLastObj, &indexLastRealObj);
	findRealObjects(data->mapPixels, data->sx, data->sy, data->eqTree, data->objects, &indexLastRealObj, data->queuePix);
	pushAndPullRealObject(data->eqTree, indexLastObj);
	repairRealObjects(data->mapPixels, data->eqTree, data->sx, data->sy);
	countCharacters(data->mapPixels, data->objects, data->sx, data->sy);
	formatMap(data->mapPixels, data->map, data->sx, data->sy);
	eqtreeClearLink(data->eqTree);

	data->countObjects = indexLastRealObj + 1;

	LOG_PRINT4("CVData. sx:%hu, sy:%hu, countObj:%i, sizeImg:%li\n", data->sx, data->sy, indexLastRealObj + 1, st.st_size);
	
  	/* Closing files */
	gdImageDestroy(im);
	fclose(pngIn);

	return data;
}

void initDataForCV(CVData *data, coord_t sx, coord_t sy, off_t sizeImg)
{
	data->mapPixels = createMapPixels(sx, sy);
	data->map = createMapPixelsEth(sx, sy);
	data->objects = (ObjParams*)malloc(sizeof(ObjParams) * sx * sy);
	data->queuePix = qpixInitQueuePixel(sx * sy);
	data->eqTree = eqtreeInitEqTree(sx * sy);
}

void freeDataForCV(CVData *data)
{
	coord_t i;
	if(data->map)
	{
		for(i = 0; i < sx; i++)
			free(data->map[i]);
		free(data->map);
	}
	if(data->objects)
		free(data->objects);
	//TODO free queue
	//TODO FULL free tree(with generated vertexes)
}


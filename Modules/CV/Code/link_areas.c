#include "Headers/link_areas.h"

/* Selection objects */

void linkAreas(gdImagePtr im, Pixel **mapPixels, coord_t sx, coord_t sy, int *indexLastObj, EqTree *eqTree, QueuePixel *queuePix)
{
	coord_t i, j;

	//TODO Unite methods "SelSq" with "SimpSel", and if inside one "SimpObj" is the only "SqObj" then \
		situation is correct(may be this algorith is correct).	\
		May be leave just "SelSq".

	
	//Selection by square
	//Check vert ways
	for(i = SIZE_SQUARE - 1; i < sx; i += SIZE_SQUARE)
		for(j = 0; j < sy - 1; j++)
			checkVertWay(im, mapPixels, i, j);

	//Check horiz ways
	for(j = SIZE_SQUARE - 1; j < sy; j += SIZE_SQUARE)
		for(i = 0; i < sx - 1; i++)
			checkHorizWay(im, mapPixels, i, j);
	

	//Simple selection
	/*for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
		{
			if(i < sx - 1)
				if(getDiff(im, mapPixels[i][j].color, mapPixels[i+1][j].color) <= ALLOWABLE_DIFF)
					mapPixels[i][j].direct |= RIGHT_WAY;
			if(i > 0)
				if(getDiff(im, mapPixels[i][j].color, mapPixels[i-1][j].color) <= ALLOWABLE_DIFF)
					mapPixels[i][j].direct |= LEFT_WAY;
			if(j < sy - 1)
				if(getDiff(im, mapPixels[i][j].color, mapPixels[i][j+1].color) <= ALLOWABLE_DIFF)
					mapPixels[i][j].direct |= DOWN_WAY;
			if(j > 0)
				if(getDiff(im, mapPixels[i][j].color, mapPixels[i][j-1].color) <= ALLOWABLE_DIFF)
					mapPixels[i][j].direct |= UP_WAY;
		}*/

	searchObjects(mapPixels, sx, sy, indexLastObj, eqTree, queuePix);

	
}

void searchObjects(Pixel **mapPixels, coord_t sx, coord_t sy, int *indexLastObj, EqTree *eqTree, QueuePixel *queuePix)
{ 
	coord_t i, j;
	for(i = 0; i < sx; i++)
		for(j = 0; j < sy; j++)
			if(mapPixels[i][j].object == IS_UNDEFINED)
				linkObject(mapPixels, sx, sy, i, j, indexLastObj, eqTree, queuePix);
}

void linkObject(Pixel **mapPixels, coord_t sx, coord_t sy, coord_t x, coord_t y, int *indexLastObj, EqTree *eqTree, QueuePixel *queuePix)
{
	Pixel curPix;

	(*indexLastObj)++;
	eqTree->vertexes[*indexLastObj].size = 0;
	eqTree->vertexes[*indexLastObj].x = x;
	eqTree->vertexes[*indexLastObj].y = y;
	qpixEnqueue(queuePix, mapPixels[x][y]);
	while(qpixIsNotEmpty(queuePix))
	{
		curPix = qpixDequeue(queuePix);
		if(mapPixels[curPix.x][curPix.y].object == IS_UNDEFINED)
		{
			eqTree->vertexes[*indexLastObj].size++;
			mapPixels[curPix.x][curPix.y].object = *indexLastObj;	//Set pixel to object


			//UP_WAY
			if(curPix.direct & UP_WAY)
				qpixEnqueue(queuePix, mapPixels[curPix.x][curPix.y - 1]);

			//RIGHT_WAY
			if(curPix.direct & RIGHT_WAY)
				qpixEnqueue(queuePix, mapPixels[curPix.x + 1][curPix.y]);

			//DOWN_WAY
			if(curPix.direct & DOWN_WAY)
				qpixEnqueue(queuePix, mapPixels[curPix.x][curPix.y + 1]);

			//LEFT_WAY
			if(curPix.direct & LEFT_WAY)
				qpixEnqueue(queuePix, mapPixels[curPix.x - 1][curPix.y]);
		}
	}
}

/* Count defference between two pixels */
void checkVertWay(gdImagePtr im, Pixel **mapPixels, coord_t i, coord_t j)
{
	coord_t k;
	byte flag = HAVE_WAY;

	for(k = i - SIZE_SQUARE + 1; k <= i; k++)
		if(getDiff(im, mapPixels[k][j].color, mapPixels[k][j + 1].color) > ALLOWABLE_DIFF)
		{
			flag = HAVE_NOT_WAY;
			break;
		}

	if(flag == HAVE_WAY)
		for(k = i - SIZE_SQUARE + 1; k <= i; k++)
		{
			mapPixels[k][j].direct |= DOWN_WAY;
			mapPixels[k][j+1].direct |= UP_WAY;
		}
}

void checkHorizWay(gdImagePtr im, Pixel **mapPixels, coord_t i, coord_t j)
{
	coord_t k;
	byte flag = HAVE_WAY;

	for(k = j - SIZE_SQUARE + 1; k <= j; k++)
		if(getDiff(im, mapPixels[i][k].color, mapPixels[i + 1][k].color) > ALLOWABLE_DIFF)
		{
			flag = HAVE_NOT_WAY;
			break;
		}


	if(flag == HAVE_WAY)
		for(k = j - SIZE_SQUARE + 1; k <= j; k++)
		{
			mapPixels[i][k].direct |= RIGHT_WAY;
			mapPixels[i+1][k].direct |= LEFT_WAY;
		}
}

int getDiff(gdImagePtr im, int pixel1, int pixel2)
{
	return (gdImageRed(im, pixel1) - gdImageRed(im, pixel2)) * (gdImageRed(im, pixel1) - gdImageRed(im, pixel2)) + 
		(gdImageGreen(im, pixel1) - gdImageGreen(im, pixel2)) * (gdImageGreen(im, pixel1) - gdImageGreen(im, pixel2)) + 
		(gdImageBlue(im, pixel1) - gdImageBlue(im, pixel2)) * (gdImageBlue(im, pixel1) - gdImageBlue(im, pixel2));
}
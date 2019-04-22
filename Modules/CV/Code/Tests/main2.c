#include <gd.h>
#include <stdlib.h>

extern gdImagePtr resImage;

//TODO Add vertexes and generate colors only for objects with size more than MIN_SIZE

//FOR DEBUG
double getCPUTime(void);

int main(void)
{
	/* Declaration */
	int i, sx, sy, indexLastObj = -1, indexLastRealObj = -1, *colors;
	double startTime, endTime;			
	gdImagePtr im;
	FILE *pngIn, *pngOut;

	/* Start working */
	startTime = getCPUTime();
	


	/* Data preparation */
	//Prepare files
	pngIn = fopen("Data/real_tst2.jpg", "rb");
	pngOut = fopen("Results/reat_tst2_res.png", "wb");
	im = gdImageCreateFromJpeg(pngIn);
	sx = gdImageSX(im);		//Can be hardcoded
	sy = gdImageSY(im);		//Can be hardcoded

	printf("Sx %i, Sy %i\n", sx, sy);

	
	/* Data processing */ 
	//gdImageSmooth(im, 0.5);		//~0.4sec
	gdImageEdgeDetectQuick(im);


	/*printf("Pseudo objects %i\n", (indexLastObj + 1));
	for(i = 0; i <= indexLastObj; i++)
		printf("Obj[%i].size:%i\n", i,  eqTree->vertexes[i].size);*/

	/*printf("Real objects %i\n", (indexLastRealObj + 1));
	colors = (int*)malloc(sizeof(int) * (indexLastRealObj + 1));
	for(i = 0; i <= indexLastRealObj; i++)
		colors[i] = gdImageColorAllocate(resImage, (i * 13) % 255, 255 - (i * 23) % 255, 255 + (49 * i) % 255);
	drawObjects(mapPixels, colors, eqTree, sx, sy);*/
	

  	/* Closing files */
	
	
	
	gdImagePng(im, pngOut);
	gdImageDestroy(im);
	printf("111\n");
	fclose(pngOut);
	fclose(pngIn);


	/* End working*/
	//ADD ALL FREE
	endTime = getCPUTime();
	fprintf(stderr, "CPU time used = %lf\n", (endTime - startTime));	

	return 0;
}

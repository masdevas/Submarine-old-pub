#include <gd.h>
#include "queue_pixel.h"

#define ALLOWABLE_DIFF 15

//FOR DEBUG
double getCPUTime();

int main()
{
	/* Declaration */
	double startTime, endTime;			
	gdImagePtr im;
	FILE *pngIn, *pngOut;
	
	/* Start working */
	startTime = getCPUTime( );		//FOR DEBUG : START TIME

	/* Data preparation */
	pngIn = fopen("Data/motorcycle.png", "rb");	//Opening of files
	pngOut = fopen("Results/motorcycle_res.png", "wb");	
	im = gdImageCreateFromPng(pngIn);

	/* Data processing */ 
	gdImageSmooth(im, 0.5);		//~0.4sec

  	/* Closing files. */
	gdImagePng(im, pngOut);		//FOR DEBUG ~0.4sec
	fclose(pngIn);
	fclose(pngOut); 

  	/* Destroy the image in memory. */
	gdImageDestroy(im);

	/* End working */
	endTime = getCPUTime( );	//FOR DEBUG : END_TIME
	fprintf(stderr, "CPU time used = %lf\n", (endTime - startTime));	//FOR DEBUG : SHOW END TIME
	return 0;
}

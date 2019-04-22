#include <gd.h> 

double getCPUTime();

int main()
{
  /* Declare the image */
  gdImagePtr im;
  /* Declare output files */
  FILE *pngin, *pngout;
  /* Declare color indexes */

double startTime, endTime;

startTime = getCPUTime( );



  pngin = fopen("test.jpg", "rb");
  pngout = fopen("res.jpg", "wb");

  /* Output the image to the disk file in PNG format. */
  im = gdImageCreateFromJpeg(pngin);

  gdImageSmooth(im, 0.5);


  /* Close the files. */
  gdImageJpeg(im, pngout, -1);
  
  fclose(pngin);

  /* Destroy the image in memory. */
  gdImageDestroy(im);

  endTime = getCPUTime( );

fprintf( stderr, "CPU time used = %lf\n", (endTime - startTime) );

  return 0;
}

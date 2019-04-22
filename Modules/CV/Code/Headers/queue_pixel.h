#ifndef QUEUE_PIXEL_MOBSEL
#define QUEUE_PIXEL_MOBSEL
//TODO FREE MEM
#include "pixel.h"

typedef struct QueuePixel
{
	Pixel *pixels;
	int start;
	int end;
	int count;
	int size;
} QueuePixel;

QueuePixel* qpixInitQueuePixel(int size);
void qpixEnqueue(QueuePixel *queue, Pixel pixel);
Pixel qpixDequeue(QueuePixel *queue);
int qpixIsEmpty(QueuePixel *queue);
int qpixIsNotEmpty(QueuePixel *queue);
int qpixIsFull(QueuePixel *queue);
int qpixGetSize(QueuePixel *queue);

#endif

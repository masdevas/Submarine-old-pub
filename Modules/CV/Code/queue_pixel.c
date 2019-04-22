#include <stdlib.h>
#include "Headers/queue_pixel.h"

QueuePixel* qpixInitQueuePixel(int size)
{
	QueuePixel *queue;
	queue = (QueuePixel*)malloc(sizeof(QueuePixel) * size);
	queue->pixels = (Pixel*)malloc(sizeof(Pixel) * size);
	queue->start = 0;
	queue->end = 0;
	queue->count = 0;
	queue->size = size;
}	//TODO Where is return?

//Non-protected from overflow
void qpixEnqueue(QueuePixel *queue, Pixel pixel)
{
	queue->pixels[queue->end] = pixel;
	queue->count++;
	if(queue->end == queue->size - 1)
		queue->end = 0;
	else
		queue->end++;
}

//Must checked for empty after using
Pixel qpixDequeue(QueuePixel *queue)
{
	queue->count--;
	if(queue->start == queue->size - 1)
	{
		queue->start = 0;
		return queue->pixels[queue->size - 1];
	}else
		return queue->pixels[queue->start++];
}

int qpixIsEmpty(QueuePixel *queue)
{
	return !queue->count;
}

int qpixIsNotEmpty(QueuePixel *queue)
{
	return queue->count;
}

int qpixIsFull(QueuePixel *queue)
{
	return queue->count == queue->size;
}

int qpixGetSize(QueuePixel *queue)
{
	return queue->size;
}
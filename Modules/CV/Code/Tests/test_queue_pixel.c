#include <stdio.h>
#include "queue_pixel.h"

void showInfo(QueuePixel *queue);

int main()
{
	QueuePixel queue = initQueuePixel(5);
	showInfo(&queue);
	Pixel p = {2, 2};
	enqueue(&queue, p);
	p.x = 3, p.y = 3;
	enqueue(&queue, p);
	p.x = 4, p.y = 4;
	enqueue(&queue, p);
	p.x = 5, p.y = 5;
	enqueue(&queue, p);
	p = dequeue(&queue);
	printf("DEQ : X%i Y%i\n", p.x, p.y);
	p.x = 6, p.y = 6;
	enqueue(&queue, p);
	p.x = 7, p.y = 7;
	enqueue(&queue, p);
	showInfo(&queue);
	return 0;
}

void showInfo(QueuePixel *queue)
{
	int i;

	printf("*******************************************************\n");
	printf("SIZE : %i\nSTART : %i\nEND : %i\nCOUNT : %i\nSIZE : %i\n", 
		queue->size, queue->start, queue->end, queue->count, queue->size);
	printf("FILL\n");
	for(i = 0; i < queue->size; i++)
		printf(" X:%i, Y:%i\n", queue->pixels[i].x, queue->pixels[i].y);
	printf("\n");

}
#include <stdio.h>
#include "../cycle_queue_bytes.h"

void showInfo(CycleQueueBytes *queue);

int main()
{
	CycleQueueBytes queue = cqbytInitCycleQueueBytes(4);
	showInfo(&queue);
	byte p = 2;
	cqbytAdd(&queue, p);
	p = 3;
	cqbytAdd(&queue, p);
	p = 4;
	cqbytAdd(&queue, p);
	p = 5;
	cqbytAdd(&queue, p);
	p = 6;
	cqbytAdd(&queue, p);
	p = 7;
	cqbytAdd(&queue, p);
	p = 8;
	cqbytAdd(&queue, p);
	p = 9;
	cqbytAdd(&queue, p);
	showInfo(&queue);
	return 0;
}

void showInfo(CycleQueueBytes *queue)
{
	int i;

	printf("*******************************************************\n");
	printf("SIZE : %i\nSTART : %i\nEND : %i\nCOUNT : %i\n", 
		queue->size, queue->start, queue->end, queue->count);
	printf("FILL\n");
	for(i = 0; i < queue->size; i++)
		printf(" %i", queue->bytes[i]);
	printf("\n");

}
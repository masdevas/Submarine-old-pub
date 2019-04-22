#ifndef VERTEX_MOBSEL
#define VERTEX_MOBSEL

#include <stdlib.h>
#include "settings.h"

typedef struct Vertex
{
	int realObject;
	int size;
	coord_t x;
	coord_t y;
	int links;
	struct Vertex *vertex;
} Vertex;

Vertex* vertInitVertex(void);

#endif
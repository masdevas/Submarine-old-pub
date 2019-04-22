#include "Headers/vertex.h"

Vertex* vertInitVertex(void)
{
	Vertex *vertex;
	vertex = (Vertex*)malloc(sizeof(Vertex));
	vertex->realObject = IS_UNDEFINED;
	vertex->size = 0;
	vertex->links = 0;
	vertex->vertex = NULL;
	return vertex;
}
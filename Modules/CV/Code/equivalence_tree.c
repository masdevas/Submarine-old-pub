#include "Headers/equivalence_tree.h"

EqTree* eqtreeInitEqTree(int _size)
{
	int i;
	EqTree *eqTree;

	eqTree = (EqTree*)malloc(sizeof(EqTree));
	eqTree->size = _size;
	eqTree->vertexes = (Vertex*)malloc(sizeof(Vertex) * _size);
	for(i = 0; i < _size; i++)
	{
		eqTree->vertexes[i].realObject = IS_UNDEFINED;
		eqTree->vertexes[i].vertex = NULL;
		eqTree->vertexes[i].size = 0;
		eqTree->vertexes[i].links = 0;
	}
	return eqTree;
}

void eqtreeLinkVertexes(EqTree *eqTree, int vertNum1, int vertNum2)
{
	Vertex *newVertex, *firstVert = eqTree->vertexes + vertNum1, *secondVert = eqTree->vertexes + vertNum2;

	if(firstVert->vertex != NULL && secondVert->vertex != NULL)
	{
		while(firstVert->vertex != NULL)
			firstVert = firstVert->vertex;

		while(secondVert->vertex != NULL)
			secondVert = secondVert->vertex;

		if(secondVert != firstVert)
		{
			firstVert->vertex = secondVert;
			secondVert->links++;
		}

	}else if(firstVert->vertex != NULL)
	{
		while(firstVert->vertex != NULL)
			firstVert = firstVert->vertex;

		secondVert->vertex = firstVert;
		firstVert->links++;

	}else if(secondVert->vertex != NULL)
	{
		while(secondVert->vertex != NULL)
			secondVert = secondVert->vertex;

		firstVert->vertex = secondVert;
		secondVert->links++;
	}else 	//If both is NULL (May be do without INIT(firstVert->vertex = secondVert, but it is may be don't work))
	{
		newVertex = vertInitVertex();
		//printf("Create %p\n", newVertex);
		firstVert->vertex = newVertex;
		secondVert->vertex = newVertex;
		newVertex->links += 2;
	}
}

void eqtreePushRealObject(EqTree *eqTree, int vertNum)
{
	Vertex *vertex = eqTree->vertexes + vertNum;

	while(vertex->vertex != NULL)
		vertex = vertex->vertex;

	vertex->realObject = eqTree->vertexes[vertNum].realObject;
}

void eqtreePullRealObjects(EqTree *eqTree)
{
	Vertex *vertex;
	int i;

	for(i = 0; i < eqTree->size; i++)
	{
		vertex = eqTree->vertexes + i;

		while(vertex->vertex != NULL)
			vertex = vertex->vertex;
		eqTree->vertexes[i].realObject = vertex->realObject;
	}
}

void eqtreePushSize(EqTree *eqTree, int vertNum)
{
	Vertex *vertex = eqTree->vertexes + vertNum;

	while(vertex->vertex != NULL)
		vertex = vertex->vertex;

	vertex->size = eqTree->vertexes[vertNum].size;
}

void eqtreePullSizes(EqTree *eqTree)
{
	Vertex *vertex;
	int i;

	for(i = 0; i < eqTree->size; i++)
	{
		vertex = eqTree->vertexes + i;

		while(vertex->vertex != NULL)
			vertex = vertex->vertex;
		eqTree->vertexes[i].size = vertex->size;
	}
}

void eqtreeClearLink(EqTree *eqTree)
{
	int i;
	byte flagRecheck = MUST_NOT_RECHECK;
	Vertex *vertex, *tmp;

	for(i = 0; i < eqTree->size; i++)
	{
		vertex = eqTree->vertexes + i;

		while(vertex->vertex)
		{
			if(vertex->vertex->links == 1)
			{
				tmp = vertex->vertex->vertex;
				//printf("Delete %p\n", vertex->vertex);
				free(vertex->vertex);
				vertex->vertex = tmp;
			}else //if vertex->vertex->links > 1
			{
				vertex->vertex->links--;
				tmp = vertex->vertex;
				vertex->vertex = NULL;
				vertex = tmp;
			}
		}
	}
}

#ifdef DEBUG

void eqtreeShowEqTree(EqTree *eqTree)
{
	int i;


	printf("SIZE:%i\n", eqTree->size);
	for(i = 0; i < eqTree->size; i++)
		printf("RealObj:%i Vert:%p Size:%i\n", eqTree->vertexes[i].realObject, eqTree->vertexes[i].vertex, 
			eqTree->vertexes[i].size);
}

#endif
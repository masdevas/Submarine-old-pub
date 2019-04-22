#ifndef EQUIV_TREE_MOBSEL
#define EQUIV_TREE_MOBSEL

#include <stdlib.h>
#include "vertex.h"

#define MUST_RECHECK 1
#define MUST_NOT_RECHECK 0

typedef struct EquivalenceTree
{
	int size;
	Vertex *vertexes;
} EqTree;

EqTree* eqtreeInitEqTree(int _size);
void eqtreeLinkVertexes(EqTree *eqTree, int vertNum1, int vertNum2);
void eqtreePushRealObject(EqTree *eqTree, int vertNum);
void eqtreePullRealObjects(EqTree *eqTree);
void eqtreePushSize(EqTree *eqTree, int vertNum);
void eqtreePullSizes(EqTree *eqTree);
void eqtreeClearLink(EqTree *eqTree);

#ifdef DEBUG

#include <stdio.h>

void eqtreeShowEqTree(EqTree *eqTree);

#endif

#endif
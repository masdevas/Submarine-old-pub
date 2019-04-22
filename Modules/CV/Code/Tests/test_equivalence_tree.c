#include "../equivalence_tree.h"

int main(void)
{
	int size = 10;
	EqTree *eqTree;

	eqTree = eqtreeInitEqTree(eqTree, 10);
	eqtreeShowEqTree(eqTree);

	eqtreeLinkVertexes(eqTree, 2, 5);
	eqtreeShowEqTree(eqTree);

	eqtreeLinkVertexes(eqTree, 4, 9);
	eqtreeShowEqTree(eqTree);

	eqtreeLinkVertexes(eqTree, 5, 9);
	eqtreeShowEqTree(eqTree);

	eqtreeLinkVertexes(eqTree, 1, 8);
	eqtreeShowEqTree(eqTree);

	eqtreeLinkVertexes(eqTree, 8, 9);
	eqtreeShowEqTree(eqTree);

	eqtreePushVertex(eqTree, 9);
	eqtreeShowEqTree(eqTree);

	eqtreePullEnds(eqTree);
	eqtreeShowEqTree(eqTree);
	return 0;
}


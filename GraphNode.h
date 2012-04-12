#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <vector>

using namespace std;

class GraphNode
{
	int numInterferences;			// how many nodes this node interferes with
	int color;						// this node's assigned color
	vector<int> adjNodes;				// integer IDs of the nodes this node interferes with
	vector<int> removedAdjNodes;		// node IDs removed during pruning

public:
	GraphNode();
	void addAdjNode(int);
	bool isAdjacentToNode(int);
   int getNumInterferences();
};

#endif

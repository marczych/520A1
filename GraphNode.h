#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <list>

using namespace std;

class GraphNode
{
	int numInterferences;			// how many nodes this node interferes with
	int color;						// this node's assigned color
	list<int> adjNodes;				// integer IDs of the nodes this node interferes with
	list<int> removedAdjNodes;		// node IDs removed during pruning

public:
	GraphNode();
	void addAdjNode(int);
};

#endif
#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include "GraphNode.h"

using namespace std;

class AdjacencyList
{
	int listLength;
	GraphNode* adjList;

	public:
		void setAdjList(GraphNode*, int);
};

#endif
#include <vector>
#include "GraphNode.h"

using namespace std;

GraphNode::GraphNode()
{
	numInterferences = 0;
	color = -1;
}

void GraphNode::addAdjNode(int id)
{
	adjNodes.push_back(id);
	++numInterferences;
}

bool GraphNode::isAdjacentToNode(int id)
{
	if (adjNodes.size() == 0) return false;

	vector<int>::iterator it;
	for (it = adjNodes.begin(); it < adjNodes.end(); it++)
		if (*it == id) return true;

	return false;
}
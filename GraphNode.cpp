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
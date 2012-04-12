#include <vector>
#include "GraphNode.h"

using namespace std;

GraphNode::GraphNode()
{
	color = -1;
}

void GraphNode::addAdjNode(int id)
{
	adjNodes.insert(id);
}

void GraphNode::removeAdjNode(int id)
{
   adjNodes.erase(id);
}

bool GraphNode::isAdjacentToNode(int id)
{
   return adjNodes.find(id) != adjNodes.end();
}

int GraphNode::getNumInterferences()
{
   return adjNodes.size();
}

set<int> GraphNode::getAdjNodes()
{
   return adjNodes;
}

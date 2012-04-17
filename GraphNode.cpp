/*
* Chris Hoover
* Marc Zych
* CSC 520 (Spring 2012): Project 1
*/


#include <iostream>
#include <vector>

#include "GraphNode.h"

using namespace std;

int GraphNode::realRegisters = -1;

GraphNode::GraphNode(int id)
{
   nodeId = id;

   // Automatically color real registers with their id
	color = nodeId < 32 ? nodeId : -1;
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
   if (nodeId < 32)
   {
      return adjNodes.size() + realRegisters;
   }
   else
   {
      return adjNodes.size();
   }
}

set<int>* GraphNode::getAdjNodes()
{
   return &adjNodes;
}

int GraphNode::getNodeId()
{
   return nodeId;
}

void GraphNode::setColor(int col)
{
   color = col;
}

int GraphNode::getColor()
{
   return color;
}

ostream& operator<<(ostream& os, const GraphNode& node)
{ 
   os << node.nodeId << ":\t";

   for (set<int>::iterator itr = node.adjNodes.begin(); itr != node.adjNodes.end(); itr++)
   {
      os << *itr << " ";
   }

   return os;
}

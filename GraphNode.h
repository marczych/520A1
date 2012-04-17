/*
* Chris Hoover
* Marc Zych
* CSC 520 (Spring 2012): Project 1
*/


#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <iostream>
#include <set>

using namespace std;

class GraphNode
{
   int nodeId;
	int color; // this node's assigned color
	set<int> adjNodes; // integer IDs of the nodes this node interferes with

public:
   static int realRegisters;

	GraphNode(int);
	void addAdjNode(int);
   void removeAdjNode(int);
   set<int>* getAdjNodes();
   int getNodeId();
   int getColor();
   void setColor(int);

	bool isAdjacentToNode(int);
   int getNumInterferences();

   void setRemoved(bool);

   friend std::ostream& operator<<(std::ostream&, const GraphNode&);
};

#endif

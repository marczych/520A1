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
	GraphNode(int);
	void addAdjNode(int);
   void removeAdjNode(int);
   set<int>* getAdjNodes();
   int getNodeId();

	bool isAdjacentToNode(int);
   int getNumInterferences();

   void setRemoved(bool);

   friend std::ostream& operator<<(std::ostream&, const GraphNode&);
};

#endif

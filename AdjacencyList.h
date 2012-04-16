#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <map>
#include <stack>
#include "GraphNode.h"

using namespace std;

class AdjacencyList
{
	map<int, GraphNode*> adjList;
   stack<GraphNode*> removedNodes;
   bool colorable;

   bool canReconstructGraph();

   static set<int> availableColors;

public:
   static int realRegisters;
   static void setRealRegisters(int);

   AdjacencyList();
   void setAdjList(map<int, GraphNode*>);
   int getListLength();
   map<int, GraphNode*>* getAdjList();
   GraphNode* getNode(int);
   void addEdge(int, int);
   void addSingleEdge(int, int);
   void removeNode(int);
   void setColorable(bool);
   void computeColorability();
   bool isColorable();
   ~AdjacencyList();

   friend std::ostream& operator<<(std::ostream&, const AdjacencyList&);
};

#endif

#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <map>
#include <vector>
#include "GraphNode.h"

using namespace std;

class AdjacencyList
{
	map<int, GraphNode*> adjList;
   vector<GraphNode*> removedNodes;

public:
   void setAdjList(map<int, GraphNode*>);
   int getListLength();
   map<int, GraphNode*>* getAdjList();
   GraphNode* getNode(int);
   void addEdge(int, int);
   void removeNode(int);
   ~AdjacencyList();

   friend std::ostream& operator<<(std::ostream&, const AdjacencyList&);
};

#endif

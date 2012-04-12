#include "AdjacencyList.h"

using namespace std;

void AdjacencyList::setAdjList(GraphNode* list, int length)
{
	adjList = list;
	listLength = length;
}

GraphNode* AdjacencyList::getAdjList() {
   return adjList;
}

int AdjacencyList::getListLength()
{
   return listLength;
}

void AdjacencyList::removeNode(int id)
{
   GraphNode* node = adjList + id;
   set<int> adjNodes = node->getAdjNodes();
   
   for (set<int>::iterator itr = adjNodes.begin(); itr != adjNodes.end(); ++itr)
   {
      (adjList + *itr)->removeAdjNode(id);
   }

   // Push the graph node onto the stack of removed nodes
}

AdjacencyList::~AdjacencyList()
{
	delete(adjList);
}

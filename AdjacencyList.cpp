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

AdjacencyList::~AdjacencyList()
{
	delete(adjList);
}

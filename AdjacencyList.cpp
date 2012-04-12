#include "AdjacencyList.h"

using namespace std;

void AdjacencyList::setAdjList(map<int, GraphNode*> list)
{
	adjList = list;
}

map<int, GraphNode*>* AdjacencyList::getAdjList() {
   return &adjList;
}

int AdjacencyList::getListLength()
{
   return adjList.size();
}

void AdjacencyList::addEdge(int first, int second)
{
   adjList[first]->addAdjNode(second);
   adjList[second]->addAdjNode(first);
}

void AdjacencyList::removeNode(int id)
{
   GraphNode* node = adjList[id];
   set<int> adjNodes = node->getAdjNodes();
   
   for (set<int>::iterator itr = adjNodes.begin(); itr != adjNodes.end(); ++itr)
   {
      adjList[*itr]->removeAdjNode(id);
   }

   // Push the graph node onto the stack of removed nodes
}

AdjacencyList::~AdjacencyList()
{
}

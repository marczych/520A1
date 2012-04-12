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

   getNode(first)->addAdjNode(second);
   getNode(second)->addAdjNode(first);
}

void AdjacencyList::removeNode(int id)
{
   GraphNode* node = getNode(id);
   set<int>* adjNodes = node->getAdjNodes();
   
   for (set<int>::iterator itr = adjNodes->begin(); itr != adjNodes->end(); ++itr)
   {
      adjList[*itr]->removeAdjNode(id);
   }

   removedNodes.push_back(node);
   adjList.erase(id);
}

GraphNode* AdjacencyList::getNode(int id)
{
   GraphNode* node;

   // I sure hope count isn't too slow
   if (adjList.count(id) == 0)
   {
      node = new GraphNode(id);
      adjList.insert(pair<int, GraphNode*>(id, node));
   }
   else
   {
      node = adjList[id];
   }

   return node;
}

AdjacencyList::~AdjacencyList()
{
}

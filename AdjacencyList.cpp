#include "AdjacencyList.h"

using namespace std;

int AdjacencyList::realRegisters = -1;

AdjacencyList::AdjacencyList()
{
   // Initialize real registers
   for (int i = 0; i < realRegisters; i ++)
   {
      getNode(i);
   }

   colorable = false;
}

bool AdjacencyList::isColorable()
{
   return colorable;
}

void AdjacencyList::setColorable(bool color)
{
   colorable = color;
}

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
   if (second > first || second < 32)
   {
      getNode(first)->addAdjNode(second);
      getNode(second)->addAdjNode(first);
   }
}

void AdjacencyList::removeNode(int id)
{
   GraphNode* node = getNode(id);
   set<int>* adjNodes = node->getAdjNodes();
   
   for (set<int>::iterator itr = adjNodes->begin(); itr != adjNodes->end(); ++itr)
   {
      getNode(*itr)->removeAdjNode(id);
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

ostream& operator<<(ostream& os, const AdjacencyList& adjList)
{ 
   os << "Adj list" << endl;

   for (map<int, GraphNode*>::const_iterator itr = adjList.adjList.begin();
    itr != adjList.adjList.end(); itr++)
   {
      os << *((*itr).second) << endl;
   }

   return os;
}

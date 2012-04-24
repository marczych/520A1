/*
* Chris Hoover
* Marc Zych
* CSC 520 (Spring 2012): Project 1
*/


#include "AdjacencyList.h"

using namespace std;

int AdjacencyList::realRegisters = -1;
set<int> AdjacencyList::availableColors;

void AdjacencyList::setRealRegisters(int registers)
{
   realRegisters = registers;

   for (int i = 0; i < realRegisters; i ++)
   {
      availableColors.insert(i);
   }
}

AdjacencyList::AdjacencyList()
{
   // Initialize real registers
   for (int i = 0; i < realRegisters; i ++)
   {
      getNode(i);
   }

   colorable = false;
}

//Commented out because it makes it significantly slower
AdjacencyList::~AdjacencyList()
{

   /*
   for (map<int, GraphNode*>::iterator itr = adjList.begin();
    itr != adjList.end(); itr++)
   {
      delete (*itr).second;
   }

   while (!removedNodes.empty())
   {
      delete removedNodes.top();
      removedNodes.pop();
   }
   */
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

void AdjacencyList::addSingleEdge(int from, int to)
{
   getNode(from)->addAdjNode(to);
}

void AdjacencyList::removeNode(int id)
{
   GraphNode* node = getNode(id);
   set<int>* adjNodes = node->getAdjNodes();
   
   for (set<int>::iterator itr = adjNodes->begin(); itr != adjNodes->end(); ++itr)
   {
      getNode(*itr)->removeAdjNode(id);
   }

   removedNodes.push(node);
   adjList.erase(id);
}

GraphNode* AdjacencyList::getNode(int id)
{
   GraphNode* node;
   map<int, GraphNode*>::iterator itr = adjList.find(id);

   if (itr == adjList.end())
   {
      node = new GraphNode(id);
      adjList.insert(pair<int, GraphNode*>(id, node));
   }
   else
   {
      node = (*itr).second;
   }

   return node;
}

void AdjacencyList::computeColorability()
{
   GraphNode* node;
   bool removedNode;
   bool optimistic = false; // True if we should optimistically remove nodes
   bool tookOptimistic = false; // True if we have removed a node optimistically
   int interferences;

   for (;;)
   {
      // The current graph is definitely colorable
      if (adjList.size() <= realRegisters)
      {
         // If we haven't removed any nodes optimistically then
         // we can definitely color the graph
         if (!tookOptimistic)
         {
            colorable = true;
            return;
         }
         else
         {
            // Graph might be colorable, lets try to reconstruct it to find out
            colorable = canReconstructGraph();
            return;
         }
      }

      removedNode = false;

      for (map<int, GraphNode*>::iterator itr = adjList.begin();
       itr != adjList.end(); itr++)
      {
         // Never touch real registers, we'll exit above if we're done
         if ((*itr).first < 32)
         {
            continue;
         }

         node = (*itr).second;
         interferences = node->getNumInterferences();

         if (interferences < realRegisters ||
          (optimistic && interferences == realRegisters))
         {
            removeNode((*itr).first);
            removedNode = true;

            if (optimistic)
            {
               tookOptimistic = true;
               optimistic = false;
               removedNode = true;

               break;
            }

            break;
         }
      }

      // Couldn't find any optimistic candidates
      if (optimistic)
      {
         colorable = false;
         return;
      }

      // Didn't find one, lets look optimistically
      if (!removedNode)
      {
         optimistic = true;
      }
   }
}

// Returns true if the graph was successfully colored during reconstruction
bool AdjacencyList::canReconstructGraph()
{
   set<int>* adjNodes;
   set<int> colors;
   GraphNode* graphNode;

   while (!removedNodes.empty())
   {
      graphNode = removedNodes.top();
      removedNodes.pop();
      adjNodes = graphNode->getAdjNodes();
      colors = availableColors; // Make a copy of static colors list

      for (set<int>::iterator adjItr = adjNodes->begin();
       adjItr != adjNodes->end(); adjItr++)
      {
         colors.erase(getNode(*adjItr)->getColor());
         addSingleEdge(*adjItr, graphNode->getNodeId());
      }

      if (colors.size() == 0)
      {
         return false;
      }
      else
      {
         graphNode->setColor(*(colors.begin()));
         adjList.insert(pair<int, GraphNode*>(graphNode->getNodeId(),
          graphNode));
      }
   }

   return true;
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

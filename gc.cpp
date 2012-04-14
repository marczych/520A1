#include <stdlib.h>
#include <iostream>

#include "GraphFileParser.h"

using namespace std;

bool isGraphColorable(AdjacencyList*, int);
bool canReconstructGraph(AdjacencyList*, int);

/*
* Note: argv[0] = path of executable.
* argv[1] should be our K value.
* argv[2] should be the graph file's name.
*/
int main(int argc, char** argv)
{
   int realRegisters = atoi(argv[1]);
	GraphFileParser parser;
	AdjacencyList* adjLists = parser.parse_infile(argv[2], realRegisters);
   bool colorable;

   //for (int i = 0; i < 27921; i ++) {
   for (int i = 0; i < 30; i ++)
   {
      colorable = isGraphColorable(adjLists + i, realRegisters);

      cout << i << "\t" << (colorable ? "true" : "false") << endl;
   }

	return 0;
}

bool isGraphColorable(AdjacencyList* adjList, int realRegisters)
{
   map<int, GraphNode*>* graphNodes = adjList->getAdjList();
   GraphNode* node;
   bool removedNode;
   bool optimistic = false; // True if we should optimistically remove nodes
   bool tookOptimistic = false; // True if we have removed a node optimistically
   int interferences;

   for (;;)
   {
      // The current graph is definitely colorable
      if (graphNodes->size() <= realRegisters)
      {
         // If we haven't removed any nodes optimistically then
         // we can definitely color the graph
         if (!tookOptimistic)
         {
            //cout << *adjList << endl;
            return true;
         }
         else
         {
            // Graph might be colorable, lets try to reconstruct it to find out
            return canReconstructGraph(adjList, realRegisters);
         }
      }

      removedNode = false;

      for (map<int, GraphNode*>::iterator itr = graphNodes->begin();
       itr != graphNodes->end(); itr++)
      {
         node = (*itr).second;
         interferences = node->getNumInterferences();

         if (interferences < realRegisters ||
          (optimistic && interferences == realRegisters))
         {
            adjList->removeNode((*itr).first);
            removedNode = true;

            if (optimistic)
            {
               tookOptimistic = true;
               optimistic = false;
               removedNode = true;

               break;
            }
         }
      }

      // Couldn't find any optimistic candidates
      if (optimistic)
      {
         return false;
      }

      // Didn't find one, lets look optimistically
      if (!removedNode)
      {
         optimistic = true;
      }
   }
}

// Returns true if the graph was successfully colored during reconstruction
bool canReconstructGraph(AdjacencyList* adjList, int realRegisters)
{
   return false;
}

#include <stdlib.h>
#include <iostream>

#include "GraphFileParser.h"

using namespace std;

bool isGraphColorable(AdjacencyList*, int);

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
   for (int i = 0; i < 38; i ++)
   {
      colorable = isGraphColorable(adjLists + i, realRegisters);

      cout << "Graph colorable: " << colorable << endl;
   }

	return 0;
}

bool isGraphColorable(AdjacencyList* adjList, int realRegisters)
{
   map<int, GraphNode*>* graphNodes = adjList->getAdjList();
   GraphNode* node;
   bool removedNode;

   do
   {
      removedNode = false;

      for (map<int, GraphNode*>::iterator itr = graphNodes->begin();
       itr != graphNodes->end(); itr++)
      {
         node = (*itr).second;

         if (node->getNumInterferences() < realRegisters)
         {
            adjList->removeNode((*itr).first);
            removedNode = true;
         }
      }
   } while (removedNode);

   // All real registers have edges to eachother
   return graphNodes->size() <= realRegisters;
}

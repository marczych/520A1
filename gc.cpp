
#include <iostream>

#include "GraphFileParser.h"

using namespace std;

void colorGraphs(AdjacencyList *, int);

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

   colorGraphs(adjLists, realRegisters);

	return 0;
}

void colorGraphs(AdjacencyList *adjLists, int realRegisters)
{
   AdjacencyList *adjList;

   //for (int i = 0; i < 27921; i ++) {
   for (int i = 0; i < 38; i ++)
   {
      adjList = adjLists + i;

      GraphNode* graphNodes = adjList->getAdjList();
      int numGraphNodes = adjList->getListLength();

      for (int j = 0; j < numGraphNodes; j ++)
      {
         cout << j << ":\t" << graphNodes[j].getNumInterferences();

         if (graphNodes[j].getNumInterferences() < realRegisters)
         {
            cout << " Pull out";
         }

         cout << endl;
      }
   }
}

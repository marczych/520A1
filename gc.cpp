#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#include "GraphFileParser.h"

using namespace std;

void colorGraph(AdjacencyList *, int);
vector<char*> getGraphStartPtrs(char*);
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
   GraphNode::realRegisters = realRegisters;
   AdjacencyList::realRegisters = realRegisters;
	GraphFileParser parser;
	AdjacencyList* adjLists = parser.parse_infile(argv[2], realRegisters);
   bool colorable;

   //for (int i = 0; i < 27921; i ++) {
   for (int i = 0; i < 32; i ++)
   {
      colorable = isGraphColorable(adjLists + i, realRegisters);

      cout << i << "\t" << (colorable ? "true" : "false") << endl;
   }

	return 0;
}

vector<char*> getGraphStartPtrs(char* graphFileName)
{
	// open a file descriptor attached to the graph file
	int fd = open(graphFileName, O_RDONLY);
	
	// get the file size
	long size = lseek(fd, 0, SEEK_END);
	//cout << "File size = " << size << endl;
	
	// create the memory map
	char* map = (char*) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
	
	vector<char*> graphStartPtrs;
	
	// construct a list of pointers to the start of graph descriptions
	// in the memory-mapped region
	for (int i = 0; i < size; ++i)
	{
		if (map[i] == 'G')
		{
			graphStartPtrs.push_back(map + i);
		}
	}
	
	return graphStartPtrs;
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

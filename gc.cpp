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

   //for (int i = 0; i < 27921; i ++) {
   for (int i = 0; i < 38; i ++)
   {
      colorGraph(adjLists + i, realRegisters);
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

void colorGraph(AdjacencyList* adjList, int realRegisters)
{
   map<int, GraphNode*>* graphNodes = adjList->getAdjList();
   GraphNode* node;

   cout << adjList->getListLength();

   for (map<int, GraphNode*>::iterator itr = graphNodes->begin();
    itr != graphNodes->end(); itr++)
   {
      node = (*itr).second;

      //cout << *node << endl;

      if (node->getNumInterferences() < realRegisters)
      {
         adjList->removeNode((*itr).first);
      }
   }

   cout << " -> " << adjList->getListLength() << endl;
}

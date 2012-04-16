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
#include <omp.h>

#include "GraphFileParser.h"

using namespace std;

void processGraphs(char*, int);
char* strpos(char*, char);

/*
* Note: argv[0] = path of executable.
* argv[1] should be our K value.
* argv[2] should be the graph file's name.
*/
int main(int argc, char** argv)
{
   int realRegisters = atoi(argv[1]);
   GraphNode::realRegisters = realRegisters;
   AdjacencyList::setRealRegisters(realRegisters);
	GraphFileParser parser;
	//AdjacencyList* adjLists = parser.parse_infile(argv[2], realRegisters);
   //bool colorable;

	processGraphs(argv[2], realRegisters);

	return 0;
}

void processGraphs(char* graphFileName, int k)
{
	// open a file descriptor attached to the graph file
	int fd = open(graphFileName, O_RDONLY);
	
	// get the file size
	long size = lseek(fd, 0, SEEK_END);
	
	// create the memory map
	char* map = (char*) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
   char* eof = map + size;

	// look through the memory-mapped region for the start of graph descriptions.
	// as soon as we find one, process that section.
	//#pragma omp parallel for shared(map)
	vector<char*> graphStartPtrs;
	for (int i = 0; i < size; ++i)
	{
		if (map[i] == 'G')
		{
			graphStartPtrs.push_back(map + i);
		}
	}
	
	cout << "Graph start pointers found." << endl;
	
	AdjacencyList* adjLists = new AdjacencyList[graphStartPtrs.size()];
	#pragma omp parallel for
	for (int i = 0; i < graphStartPtrs.size(); ++i)
	{
		AdjacencyList* adjList = adjLists + i;
		char* graph = graphStartPtrs[i];
		
		graph = strpos(graph, ' ') + 1; // Find graph number
		char* endOfLine;
		int graphNum = atoi(graph);
		int nodeId;

		graph = strpos(graph, '\n') + 1;
		graph = strpos(graph, '\n') + 1; // Skip the "K=XX" line

		// Reads in nodes and their neighboring nodes.
		// Each iteration starts after a '\n'.
		for (; *graph != 'G' && graph < eof &&
		// '3<->32 2<->33' etc. signals end of graph
		*(strpos(graph, '-') - 1) != '<'; graph++)
		{
			nodeId = atoi(graph);
			graph = strpos(graph, '>') + 2; // Go to start of interferences

			for (endOfLine = strpos(graph, '\n'); graph < endOfLine;
			graph = strpos(graph, ' ') + 1)
			{
				adjList->addEdge(nodeId, atoi(graph));
			}
		}
	}
	
	cout << "Adjacency lists created." << endl;
	
	#pragma omp parallel for
	for (int i = 0; i < graphStartPtrs.size(); ++i)
	{
		adjLists[i].computeColorability();
	}

	for (int i = 0; i < graphStartPtrs.size(); ++i)
   {
      cout << i+1 << ":\t" << (adjLists[i].isColorable() ? "Colorable" :
       "Uncolorable") << endl;
   }
}

char* strpos(char* haystack, char needle)
{
   for (; *haystack != needle; haystack++);

   return haystack;
}

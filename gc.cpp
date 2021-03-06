/*
* Chris Hoover
* Marc Zych
* CSC 520 (Spring 2012): Project 1
*/


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
#include <fstream>
#include <sstream>

#include "GraphNode.h"
#include "AdjacencyList.h"

using namespace std;

void workOnGraph(char*, bool*);
void processGraphs(char*, int);
char* strpos(char*, char);
void writeResultsToFile(int, bool[], int);

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

	processGraphs(argv[2], realRegisters);

	return 0;
}

void workOnGraph(char* graph, bool* results)
{
   AdjacencyList* adjList = new AdjacencyList();
   
   graph = strpos(graph, ' ') + 1; // Find graph number
   char* endOfLine;
   int graphNum = atoi(graph);
   int nodeId;

   graph = strpos(graph, '\n') + 1;
   graph = strpos(graph, '\n') + 1; // Skip the "K=XX" line

   // Reads in nodes and their neighboring nodes.
   // Each iteration starts after a '\n'.
   for (; *graph != 'G' && 
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

   adjList->computeColorability();
   results[graphNum] = adjList->isColorable();

   delete adjList;
}

void writeResultsToFile(int k, bool results[], int numGraphs)
{
	ofstream outFile;
	string outName;
	
	// Convert our K value to a string
	stringstream temp;
	temp << k;
	outName = temp.str();
	
	// Finish making the output file name, then open the file.
	outName += "_results.txt";
	outFile.open(outName.c_str());
	
	for (int i = 0; i < numGraphs; ++i)
	{
		outFile << "Graph " << i+1 << ": ";
		
		if (results[i])
			outFile << "Colorable";
		else
			outFile << "Not Colorable";
		
		outFile << endl;
	}
}

char* strpos(char* haystack, char needle)
{
   for (; *haystack != needle; haystack++);

   return haystack;
}

void processGraphs(char* graphFileName, int k)
{
	// open a file descriptor attached to the graph file
	int fd = open(graphFileName, O_RDONLY);
   bool results[30000];

	// get the file size
	long size = lseek(fd, 0, SEEK_END);
	
	// create the memory map
	char* map = (char*) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

	// look through the memory-mapped region for the start of graph descriptions.
	// as soon as we find one, process that section.
   #pragma omp parallel for
	for (int i = 0; i < size; ++i)
	{
		if (map[i] == 'G')
		{
         workOnGraph(map + i, results);
		}
	}
	
	writeResultsToFile(k, results, 27000);
}

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

#include "GraphFileParser.h"

using namespace std;

void colorGraph(AdjacencyList *, int);
void processGraphs(char*, int);
bool isGraphColorable(AdjacencyList*, int);
bool canReconstructGraph(AdjacencyList*, int);
void workOnGraph(char*, int);
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
   AdjacencyList::realRegisters = realRegisters;
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

	// look through the memory-mapped region for the start of graph descriptions.
	// as soon as we find one, process that section.
	//#pragma omp parallel for shared(map)
	vector<char*> graphStartPtrs;
	for (int i = 0; i < size; ++i)
	{
		if (map[i] == 'G')
		{
			//workOnGraph(map + i, k);
			graphStartPtrs.push_back(map + i);
		}
	}
	
	const int numGraphs = graphStartPtrs.size();
	cout << "Graph start pointers found." << endl;
	
	AdjacencyList* adjLists = new AdjacencyList[graphStartPtrs.size()];
	#pragma omp parallel for
	for (int i = 0; i < numGraphs; ++i)
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
		for (; *graph != 'G' && *graph != '\0' &&
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
	
	bool results[numGraphs];
	
	#pragma omp parallel for
	for (int i = 0; i < numGraphs; ++i)
	{
		adjLists[i].setColorable(isGraphColorable(adjLists + i, k));
		results[i] = adjLists[i].isColorable();
	}
	
	// create output file
	writeResultsToFile(k, results, numGraphs);
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

void workOnGraph(char* graph, int realRegisters)
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
   for (; *graph != 'G' && *graph != '\0' &&
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

char* strpos(char* haystack, char needle)
{
   for (; *haystack != needle; haystack++);

   return haystack;
}

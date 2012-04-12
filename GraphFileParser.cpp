#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "GraphFileParser.h"
#include "GraphNode.h"
#include "AdjacencyList.h"

using namespace std;

/*
* Reads the input file and returns an AdjacencyList array, where element x
* corresponds to the graph labeled x+1 in the input file.
*
* NOTE: The AdjacencyList array is heap-allocated, so the caller must free
* its memory when it is no longer needed.
*/
AdjacencyList* GraphFileParser::parse_infile(char* fileName, int k)
{
	bool firstNewGraph = true;
	vector<string> lines;		// stores the lines for a single graph from the input file.
	ifstream inFile;
	string line;

	AdjacencyList* adjLists = new AdjacencyList[27921]; // would probably be better to dynamically detect this number...
	int graphNum = 0;

	inFile.open(fileName);
	if (inFile.is_open())
	{
		while (inFile.good())
		{
			getline(inFile, line);
			
			if (line.compare(0, 5, "Graph", 5) == 0)
			{
				// The current line in the file marks the start of a new graph.

				if (!firstNewGraph)
				{
					// We've already collected some lines for a graph. Process those
					// before we collect any more.
					cout << "Processing graph " << graphNum << endl;

					int listLength;
					GraphNode* adjList = process_lines(lines, listLength, k);

					adjLists[graphNum].setAdjList(adjList, listLength);

					lines.clear();
					++graphNum;
				}
				else
				{
					// No processing needed if this is the start of the file's first graph.
					firstNewGraph = false;
				}			
			}
			else if ((line.compare(0, 2, "K=", 2) != 0)	&& (line.find("<") == string::npos))
			{
				// There's no way to know how many lines are given for this graph in the file,
				// so just save the lines for later as we find them. When we find the start
				// of a new graph, we'll process these saved lines before moving on.
				lines.push_back(line);
			}
		}
		inFile.close();
	}

	// Return some sort of collection that holds all the adjacency lists
	// constructed while reading the file
	return adjLists;
}

/*
* Given a list of explicit interference lines from the input file, constructs
* an adjacency list for that graph, represented as an array of GraphNode objects.
* 
* NOTE: The GraphNode array is heap-allocated, and must be freed by the caller.
*/
GraphNode* GraphFileParser::process_lines(vector<string> lines, int& listLength, int k)
{
	// If lines is empty, we still need space for the "real registers" (nodes 0..K-1).
	if (lines.size() == 0)
	{
		listLength = k;
	}
	else
	{
		// Do a pass through the lines to see what the highest explicitly referenced
		// node ID is. This is necessary since a node's integer ID doubles as the position
		// in the adjacency list where its GraphNode object is stored. Indices 0..<maxId>
		// must be valid.
		int maxId = 0;
		for (int i = 0; i < lines.size(); ++i)
		{
			string line = lines[i];
			string token;
			stringstream ss(line);

			if (ss >> token && atoi(token.c_str()) > maxId)
				maxId = atoi(token.c_str());
		}

		listLength = maxId + 1;
	}

	GraphNode* adjList = new GraphNode[listLength];

	for (int i = 0; i < lines.size(); ++i)
	{

		string line = lines[i];
		string token;
		int tokenCount = 0;
		stringstream ss(line); 

		int mainNodeId = -1;

		// tokenize line (use " " as delimiter)
		while (ss >> token)
		{
			if (token.compare(0, 3, "-->", 3) != 0)
			{
				int nodeId = atoi(token.c_str());

				if (tokenCount == 0)
				{
					// new graph node
					mainNodeId = nodeId;
				}
				else
				{
					// node ID in adjacency list
					adjList[mainNodeId].addAdjNode(nodeId);
					adjList[nodeId].addAdjNode(mainNodeId);
				}
			}
			++tokenCount;
		}
	}

	// Now add explicit interferences for nodes 0..K-1
	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < k; ++j)
		{
			// Make sure we don't add duplicate interferences.
			if (i != j && !adjList[i].isAdjacentToNode(i) && !adjList[i].isAdjacentToNode(j))
			{
				adjList[i].addAdjNode(j);
				adjList[j].addAdjNode(i);
			}
		}
	}

	return adjList;
}

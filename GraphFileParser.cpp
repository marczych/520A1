#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "GraphFileParser.h"
#include "GraphNode.h"
#include "AdjacencyList.h"

using namespace std;

void GraphFileParser::parse_infile(char* fileName)
{
	bool firstNewGraph = true;
	vector<string> lines;
	ifstream inFile;
	string line;

	AdjacencyList adjLists[27921]; // would probably be better to dynamically detect this number...
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
					GraphNode* adjList = process_lines(lines, listLength);

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

		// Return some sort of collection that holds all the adjacency lists
		// constructed while reading the file
	}
}

GraphNode* GraphFileParser::process_lines(vector<string> lines, int& listLength)
{
	// Do a pass through the lines to see what the highest explicitly referenced
	// node ID is.
	int maxId = 0;
	for (int i = 0; i < lines.size(); ++i)
	{
		string line = lines[i];
		string token;
		stringstream ss(line);

		if (ss >> token && atoi(token.c_str()) > maxId)
			maxId = atoi(token.c_str());
	}

	GraphNode* adjList = new GraphNode[maxId + 1];

	for (int i = 0; i < lines.size(); ++i)
	{
		// tokenize line (use " " as delimiter)

		// node index <-- 1st token

		// list of indices this node interferes with <-- 3rd, 4th, ... tokens (2nd token is the "-->"

		string line = lines[i];
		string token;
		int tokenCount = 0;
		stringstream ss(line); 

		int mainNodeId = -1;

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

	// All done... yield an array of GraphNode objects
	// where the array indices correspond to the node IDs from the file.
	// This is essentially an adjacency list for the graph.
	return adjList;
}
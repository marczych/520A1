#ifndef GRAPH_FILE_PARSER_H
#define GRAPH_FILE_PARSER_H

#include <vector>
#include "GraphNode.h"
#include "AdjacencyList.h"

using namespace std;

class GraphFileParser
{
public:
	AdjacencyList* parse_infile(char*, int);

private:
	GraphNode* process_lines(vector<string>, int&, int);
};

#endif

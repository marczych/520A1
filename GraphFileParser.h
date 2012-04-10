#ifndef GRAPH_FILE_PARSER_H
#define GRAPH_FILE_PARSER_H

#include <vector>
#include "GraphNode.h"

using namespace std;

class GraphFileParser
{
public:
	void parse_infile(char*);

private:
	GraphNode* process_lines(vector<string>, int&);
};

#endif
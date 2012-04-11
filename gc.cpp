
#include <iostream>

#include "GraphFileParser.h"

using namespace std;

/*
* Note: argv[0] = path of executable.
* argv[1] should be our K value.
* argv[2] should be the graph file's name.
*/
int main(int argc, char** argv)
{
	GraphFileParser parser;
	AdjacencyList* adjLists = parser.parse_infile(argv[2]);

	return 0;
}

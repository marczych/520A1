

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GraphFileParser.h"

using namespace std;

int main(int argc, char** argv)
{
	GraphFileParser parser;
	parser.parse_infile(argv[2]);

   return 0;
}

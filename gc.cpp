

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GraphFileParser.h"

using namespace std;

void main(int argc, char** argv)
{
	GraphFileParser parser;
	parser.parse_infile(argv[2]);
}
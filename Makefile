gc: gc.o GraphFileParser.o GraphNode.o AdjacencyList.o
	g++ -g -o gc gc.o GraphFileParser.o GraphNode.o AdjacencyList.o

gc.o: gc.cpp GraphFileParser.h
	g++ -g -c gc.cpp

GraphFileParser.o: GraphFileParser.cpp GraphFileParser.h GraphNode.h AdjacencyList.h
	g++ -g -c GraphFileParser.cpp

GraphNode.o: GraphNode.cpp GraphNode.h
	g++ -g -c GraphNode.cpp

AdjacencyList.o: AdjacencyList.cpp AdjacencyList.h GraphNode.h
	g++ -g -c AdjacencyList.cpp

clean:
	rm *.o gc

gc: gc.o GraphFileParser.o GraphNode.o AdjacencyList.o
	g++ -fopenmp -O3 -o gc gc.o GraphFileParser.o GraphNode.o AdjacencyList.o

gc.o: gc.cpp GraphFileParser.h
	g++ -fopenmp -O3 -c gc.cpp

GraphFileParser.o: GraphFileParser.cpp GraphFileParser.h GraphNode.h AdjacencyList.h
	g++ -fopenmp -O3 -c GraphFileParser.cpp

GraphNode.o: GraphNode.cpp GraphNode.h
	g++ -fopenmp -O3 -c GraphNode.cpp

AdjacencyList.o: AdjacencyList.cpp AdjacencyList.h GraphNode.h
	g++ -fopenmp -O3 -c AdjacencyList.cpp

clean:
	rm *.o gc

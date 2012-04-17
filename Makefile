gc: gc.o GraphNode.o AdjacencyList.o
	g++ -fopenmp -O3 -o gc gc.o GraphNode.o AdjacencyList.o

gc.o: gc.cpp GraphNode.h AdjacencyList.h
	g++ -fopenmp -O3 -c gc.cpp

GraphNode.o: GraphNode.cpp GraphNode.h
	g++ -fopenmp -O3 -c GraphNode.cpp

AdjacencyList.o: AdjacencyList.cpp AdjacencyList.h GraphNode.h
	g++ -fopenmp -O3 -c AdjacencyList.cpp

clean:
	rm *.o gc

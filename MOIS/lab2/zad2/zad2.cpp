#include "Graph.h"

int main()
{
	ifstream inp("input.txt");

	if (inp.is_open()) {
		int numV, numAdj;
		inp >> numV >> numAdj;
		Graph g(numV);
		for (int i = 0; i < numAdj; i++) {
			int a, b;
			inp >> a >> b;
			g.addEdge(a - 1, b - 1);
		}
		g.hamiltonianCycle();

	}
	else cout << "file not found";

	return 0;
}
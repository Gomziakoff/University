#include "Graph.h"

int main()
{
	ifstream inp("input.txt");
	ofstream out("output.txt");
	if (inp.is_open()) {
		int numV, numAdj;
		inp >> numV >> numAdj;
		Graph g(numV);
		for (int i = 0; i < numAdj; i++) {
			int a, b;
			inp >> a >> b;
			g.addEdge(a-1, b-1);
		}
		out << g.num_comp_DFS() << " - число компонент связности вычисленное методом поиска в глубину" << endl << g.num_comp_BFS() << " - число компонент связности вычисленное методом поиска в ширину";

	}
	else cout << "file not found";

	

	return 0;
}
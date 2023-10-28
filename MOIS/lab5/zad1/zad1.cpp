#include "Graph.h"

using namespace std;

int main() {
	ifstream inp("input.txt");

	if (inp.is_open()) {
		int numV, numAdj;
		inp >> numV >> numAdj;
		Graph g(numV);
		for (int i = 0; i < numAdj; i++) {
			int a, b, weight;
			inp >> a >> b >> weight;
			g.addEdge(a, b , weight);
		}
		vector<vector<int>> tree1, tree2;
		tree1 = g.prim();
		vector<int> couple = g.couple_from_adjancy(tree1);
		cout << "Prim:";
		g.print(couple);
		cout << endl << "Kruskal:\n";
		tree2 = g.kruskal();
		g.showGraph(tree2);
	}
	else cout << "file not found";

	return 0;
}
#include "Graph.h"

int main() {
	ifstream inp("input.txt");

	if (inp.is_open()) {
		int numV;
		inp >> numV;
		Graph g(numV);
		for (;!inp.eof();) {
			int a, b;
			inp >> a >> b;
			g.addEdge(a, b);
		}
		
	}
	else cout << "file not found";
}
#include <iostream>
#include <list>

using namespace std;

class Graph {
	int numvert;
	list<int>* adjLists;
	bool* visited;

public:
	Graph(int V);
	void addEdge(int src, int dest);
	void DFS(int vertex, bool visited[]);
	void BFS(int vertex, bool visited[]);
	int CCC();
};

Graph::Graph(int V)
{
	numvert = V;
	adjLists = new list<int>[V];
	visited = new bool[V];
}

void Graph::addEdge(int src, int dest)
{
	adjLists[src].push_front(dest);
	adjLists[dest].push_back(src);
}

void Graph::DFS(int vertex, bool visited[]) //deep search
{
	visited[vertex] = true;
	list<int>::iterator i;
	for (i = adjLists[vertex].begin(); i != adjLists[vertex].end(); ++i)
		if (!visited[*i])
			DFS(*i, visited);
}

void Graph::BFS(int vertex, bool visited[])
{
	list<int> queue;
	visited[vertex] = true;
	queue.push_back(vertex);

	list<int>::iterator i;

	while (!queue.empty()) {
		int currVertex = queue.front();
		queue.pop_front();

		for (i = adjLists[currVertex].begin(); i != adjLists[currVertex].end(); ++i)
		{
			int adjVertex = *i;
			if (!visited[adjVertex])
			{
				visited[adjVertex] = true;
				queue.push_back(adjVertex);
			}
		}
	}

}

int Graph::CCC() {
	int count = 0;
	bool* visited = new bool[numvert];

	for (int i = 0; i < numvert; ++i) {
		visited[i] = false;
	}

	for (int i = 0; i < numvert; ++i) {
		if (!visited[i]) {
			DFS(i,visited);
			// Или можно использовать BFS вместо DFS:
			// BFS(i, visited);
			count++;
		}
	}

	delete[] visited;
	return count;
}

int main()
{
	Graph g(4);
	g.addEdge(0, 0);
	g.addEdge(2, 2);
	g.addEdge(1, 1);
	g.addEdge(2, 3);

	cout << g.CCC();

	return 0;
}
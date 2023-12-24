#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <vector>

using namespace std;

class Graph {
	int numvert;
	list<int>* adjLists;
	bool* visited;
	vector<int> path;
public:
	Graph(int V);
	void addEdge(int src, int dest);
	void DFS(int vertex, bool visited[]);
	bool is_euralian();
	void findcycle(int V, vector<int>& way);
	void showcycle();
	bool hamiltonianCycle();
	bool findHamiltonian(int v);
};

Graph::Graph(int V)
{
	numvert = V;
	adjLists = new list<int>[V];
	visited = new bool[V];
}

void Graph::addEdge(int src, int dest)
{
	adjLists[src].push_back(dest);
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

bool Graph::is_euralian()
{
	int num = 0;
	for (int i = 0; i < numvert; i++) {
		if (adjLists[i].size() == 0) return false;
		if (adjLists[i].size() % 2 != 0) num++;
	}
	if (num == 0) return true; else return false;
}

void Graph::findcycle(int V, vector<int>& way) {
	while (!adjLists[V].empty()) {
		int u = adjLists[V].front();
		for (auto i = adjLists[V].begin(); i != adjLists[V].end(); ++i) {
			if (*i == u) {
				adjLists[V].erase(i);
				break;
			}
		}
		for (auto i = adjLists[u].begin(); i != adjLists[u].end(); ++i) {
			if (*i == V) {
				adjLists[u].erase(i);
				break;
			}
		}
		findcycle(u, way);
	}
	way.push_back(V);
}

void Graph::showcycle() {
	if (!is_euralian()) {
		cout << "graph is not euralian" << endl;
		return;
	}
	ofstream out("output.txt");
	int start = 0;
	vector<int> way;

	findcycle(start, way);

	cout << "эйлеров цикл: ";

	for (int a : way) {
		cout << a + 1 << " ";
		out << a + 1 << " ";
	}

	cout << endl;
}

bool Graph::hamiltonianCycle() {
	ofstream out("output.txt");
	for (int i = 0; i < numvert; ++i) {
		visited[i] = false;
	}
	path.push_back(0);
	visited[0] = true;

	if (findHamiltonian(0)) {
		cout << "Гамильтонов цикл: ";
		for (int vertex : path) {
			cout << vertex << " ";
			out << vertex << " ";
		}
		cout << path[0]+1;
		out << path[0] << "\n";
		return true;
	}

	cout << "Гамильтонов цикл не найден.\n";
	return false;
}

bool Graph::findHamiltonian(int v) {
	if (path.size() == numvert) {
		int lastVertex = path.back();
		int firstVertex = path[0];
		for (int neighbor : adjLists[lastVertex]) {
			if (neighbor == firstVertex) {
				return true;
			}
		}
		return false;
	}

	for (int neighbor : adjLists[v]) {
		if (!visited[neighbor]) {
			visited[neighbor] = true;
			path.push_back(neighbor);

			if (findHamiltonian(neighbor)) {
				return true;
			}

			visited[neighbor] = false;
			path.pop_back();
		}
	}

	return false;
}
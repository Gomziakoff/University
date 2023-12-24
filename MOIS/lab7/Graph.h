#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

class Graph {
	int numvert;
	list<int>* adjLists;
	bool* visited;
	vector<int> hash;
	unordered_map<int, int> nodeMap;
public:
	Graph(int V);
	void addEdge(int src, int dest);
	void DFS(int startVertex);
	void ShowAdjMatrix();
	void ShowIntendenceMatrix();
	vector<int> getHash();
	void ShowTree();
private:
	void printTreeRecursive(int vertex, int level, std::unordered_map<int, int>& nodeMap, bool isLastChild);
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
}

void Graph::DFS(int startVertex) //deep search
{
	stack<int> stack;
	stack.push(startVertex);
	visited[startVertex] = true;

	while (!stack.empty()) {
		int currentVertex = stack.top();
stack.pop();
		hash.push_back(currentVertex);

		for (int neighbor : adjLists[currentVertex]) {
				stack.push(neighbor);
				visited[neighbor] = true;
		}
	}
}

vector<int> Graph::getHash() {
	return hash;
}

void Graph::ShowAdjMatrix() {
	ofstream ans("output_zad1.txt");
	vector<vector<int>> a(numvert, vector<int>(numvert, 0));
	list<int>::iterator j;
	for (int i = 0; i < numvert; i++) {
		for (j = adjLists[i].begin(); j != adjLists[i].end(); ++j) {
			a[i][*j] = 1;
		}
	}
	for (int i = 0; i < numvert; i++) {
		for (int j = 0; j < numvert; j++) {
			cout << a[i][j];
			ans << a[i][j];
		}
		cout << endl;
	}
}

void Graph::ShowIntendenceMatrix() {
	ofstream ans("output_zad1.txt");
	int numEdges = 0;
	for (int i = 0; i < numvert; i++) {
		numEdges += adjLists[i].size();
	}
	vector<vector<int>> incidenceMatrix(numvert, vector<int>(numEdges, 0));
	int edgeIndex = 0;
	for (int i = 0; i < numvert; i++) {
		for (const int& neighbor : adjLists[i]) {
			incidenceMatrix[i][edgeIndex] = 1;
			incidenceMatrix[neighbor][edgeIndex] = 1;
			edgeIndex++;
		}
	}
	for (int i = 0; i < numvert; i++) {
		for (int j = 0; j < numEdges; j++) {
			cout << incidenceMatrix[i][j];
			ans << incidenceMatrix[i][j];
		}
		cout << endl;
	}
}

void Graph::ShowTree() {
	std::cout << "Tree Structure:" << std::endl;
	printTreeRecursive(0, 0, nodeMap, true);
}

void Graph::printTreeRecursive(int vertex, int level, std::unordered_map<int, int>& nodeMap, bool isLastChild) {
	for (int i = 0; i < level; ++i) {
		if (i == level - 1 && !isLastChild) {
			std::cout << "├─";  // Ветка
		}
		else if (i == level - 1 && isLastChild) {
			std::cout << "└─";  // Последняя ветка
		}
		else {
			std::cout << "   ";  // Промежуток до следующей вершины
		}
	}
	std::cout << "Vertex " << vertex << std::endl;

	int numChildren = adjLists[vertex].size();
	int currentChild = 0;

	for (int neighbor : adjLists[vertex]) {
		
		printTreeRecursive(neighbor, level + 1, nodeMap, ++currentChild == numChildren);
	}
}


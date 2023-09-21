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
public:
	Graph(int V);
	void addEdge(int src, int dest);
	void DFS(int vertex, bool visited[]);
	void BFS(int vertex, bool visited[]);
	int num_comp_DFS();
	int num_comp_BFS();
	void ShowAdjMatrix();
	void ShowIntendenceMatrix();
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
			if (!visited[*i])
			{
				visited[*i] = true;
				queue.push_back(*i);
			}
		}
	}
}

int Graph::num_comp_DFS() {
	int count = 0;
	bool* visited = new bool[numvert];
	for (int i = 0; i < numvert; ++i) {
		visited[i] = false;
	}
	for (int i = 0; i < numvert; ++i) {
		if (!visited[i]) {
			DFS(i, visited);
			count++;
		}
	}
	delete[] visited;
	return count;
}

int Graph::num_comp_BFS() {
	int count = 0;
	bool* visited = new bool[numvert];
	for (int i = 0; i < numvert; ++i) {
		visited[i] = false;
	}
	for (int i = 0; i < numvert; ++i) {
		if (!visited[i]) {
			BFS(i, visited);
			count++;
		}
	}
	delete[] visited;
	return count;
}

void Graph::ShowAdjMatrix() {
	ofstream ans("AdjMatrix.txt");
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
		ans << endl;
	}
}

void Graph::ShowIntendenceMatrix() {
	ofstream ans("IncidenceMatrix.txt");
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
		ans << endl;
	}
}
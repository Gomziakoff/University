#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Graph {
	int numvert;
	vector<vector<int>> adjacencyMatrix;
	bool* visited;

public:
	Graph(int V);
	void addEdge(int src, int dest);
	void showGraph(vector<vector<int>> adjancyMatrix);
	vector<vector<int>> prim();
	vector<vector<int>> kruskal();
	static vector<int> couple_from_adjancy(vector<vector<int>> adjacencyMatrix)
	{
		vector<int> couple;
		int max_node = adjacencyMatrix.size();
		for (int i = 0; i < max_node; i++)
		{
			for (int j = i + 1; j < max_node; j++)
			{
				if (adjacencyMatrix[i][j])
				{
					couple.push_back(i );
					couple.push_back(j );
				}
			}
		}
		return couple;
	}
	void print(vector<int> nodes)
	{
		int n = nodes.size();
		for (int i = 0; i < n; i++)
		{
			if (i % 2 == 0)
			{
				cout << "(" << nodes[i]  << ",";
			}
			else if (i % 2 == 1 && i != n - 1)
			{
				cout << nodes[i]  << "),";
			}
			else
			{
				cout << nodes[i]  << ")";
			}
		}
	}
};

Graph::Graph(int V)
{
	numvert = V;
	adjacencyMatrix.assign(V, vector<int>(V));
	visited = new bool[V];
}

void Graph::addEdge(int src, int dest)
{
	adjacencyMatrix[src][dest] = 1;
	
}

void Graph::showGraph(vector<vector<int>> Matrix)
{
	for (int i = 0; i < numvert; i++) {
		for (int j = 0; j < 6; j++) {
			cout << Matrix[i][j]<<" ";
		}
		cout << endl;
	}
}

vector<vector<int>> Graph::prim()
{
	vector<vector<int>> tree(numvert, vector<int>(numvert));
	vector<int> key(numvert);
	vector<int> parent(numvert);
	vector<int> visited(numvert);
	for (int i = 0; i < numvert; i++) {
		key[i] = INT_MAX;
		visited[i] = 0;
	}
	key[0] = 0;
	parent[0] = -1;
	for (int i = 0; i < numvert - 1; i++) {
		int min = INT_MAX;
		int min_index = 0;
		for (int j = 0; j < numvert; j++) {
			if (visited[j] == 0 && key[j] < min) {
				min = key[j];
				min_index = j;
			}
		}
		visited[min_index] = 1;
		for (int j = 0; j < numvert; j++) {
			if (visited[j] == 0 && adjacencyMatrix[min_index][j] && adjacencyMatrix[min_index][j] < key[j]) {
				parent[j] = min_index;
				key[j] = adjacencyMatrix[min_index][j];
			}
		}
	}
	for (int i = 1; i < numvert; i++) {
		tree[i][parent[i]] = adjacencyMatrix[i][parent[i]];
		tree[parent[i]][i] = adjacencyMatrix[i][parent[i]];
	}
	return tree;
}

inline vector<vector<int>> Graph::kruskal()
{
	vector<vector<int>> tree(numvert, vector<int>(numvert));
	set<set<int>> nodes;
	vector<vector<int>> edges;
	for (int i = 0; i < numvert; i++)
	{
		for (int j = i + 1; j < numvert; j++)
		{
			if (adjacencyMatrix[i][j])
			{
				edges.push_back({ adjacencyMatrix[i][j], i, j });
			}
		}
	}
	sort(edges.begin(), edges.end());

	for (int i = 0; i < numvert; i++)
	{
		nodes.insert(set<int>{i});
	}

	for (int i = 0; i < edges.size(); i++)
	{
		set<int> node1;
		set<int> node2;
		for (auto it = nodes.begin(); it != nodes.end(); it++)
		{
			if (it->find(edges[i][1]) != it->end())
			{
				node1 = *it;
			}
			if (it->find(edges[i][2]) != it->end())
			{
				node2 = *it;
			}
		}
		if (node1 != node2)
		{
			nodes.erase(node1);
			nodes.erase(node2);
			node1.insert(node2.begin(), node2.end());
			nodes.insert(node1);
			tree[edges[i][1]][edges[i][2]] = edges[i][0];
			tree[edges[i][2]][edges[i][1]] = edges[i][0];
		}
	}
	return tree;
}




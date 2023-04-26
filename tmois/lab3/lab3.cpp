#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const int INF = 999999;
vector<pair<int, int>> smej[1000];
int dist[1000]{};

void dekstra(int start) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	pq.push(make_pair(0, start));
	dist[start] = 0;
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();

		for (auto v : smej[u]) {
			int w = v.first;
			int to = v.second;

			if (dist[u] + w < dist[to]) {
				dist[to] = dist[u] + w;
				pq.push(make_pair(dist[to], to));
			}
		}
	}
}



int main() {
	cout << "введите кол-во вершин графа, кол-во ребер\n";
	int v, r, start;
	cin >> v >> r;
	int graph[100][100];
	for (int i = 1; i <= v; ++i) {
		dist[i] = INF;
		for (int j = 1; j <= v; j++) {
			graph[i][j] = INF;
			if (i == j) graph[i][j] = 0;
		}
	}
	cout << "введите вершины и вес ребра\n";
	for (int i = 1; i <= r;i++) {
		int u, v, w;
		cin >> u >> v >> w;
		smej[u].push_back(make_pair(w, v));
		graph[u][v] = w;
	}
	int choise;
	cout << "1.deikstra\n2.orshala\n";
	cin >> choise;
	
	switch (choise) {
	case 1:
		cout << "введите стартовую точку";
		cin >> start;
		dekstra(start);
		for (int i = 1; i <= v; ++i) {
			if (dist[i] == INF) {
				cout << "INF" << endl;
				continue;
			}
			cout << "x" << i << " = " << dist[i] << endl;
		}
		break;
	case 2:
		for (int k = 1; k <= v; k++) {
			for (int i = 1; i <= v; i++) {
				for (int j = 1; j <= v; j++) {
					if (graph[i][j] > graph[i][k] + graph[k][j]) {
						graph[i][j] = graph[i][k] + graph[k][j];
					}
				}
			}
		}
		for (int i = 1; i <= v; i++) {
			for (int j = 1; j <= v; j++) {
				if (i == j) continue;
				cout << i << " -> " << j << " = " << graph[i][j]<<endl;
			}
		}
		break;
	}
	
}
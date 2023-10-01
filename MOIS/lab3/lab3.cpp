#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>

using namespace std;

const int INF = 999999;
vector<pair<int, int>> smej[1000];
int dist[1000]{};
int parent[1000]{};
void dekstra(int start) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	pq.push(make_pair(0, start));
	dist[start] = 0;
	parent[start] = -1;
	while (!pq.empty()) {
		int u = pq.top().second;
		pq.pop();

		for (auto v : smej[u]) {
			int w = v.first;
			int to = v.second;

			if (dist[u] + w < dist[to]) {
				dist[to] = dist[u] + w;
				pq.push(make_pair(dist[to], to));
				parent[to] = u;
			}
		}
	}
}



int main() {
	ifstream file("input.txt");
	int v, r;
	file >> v >> r;
	int graph[100][100];
	for (int i = 1; i <= v; ++i) {
		dist[i] = INF;
		for (int j = 1; j <= v; j++) {
			graph[i][j] = INF;
			if (i == j) graph[i][j] = 0;
		}
	}
	for (int i = 1; i <= r; i++) {
		int u, v, w;
		file >> u >> v >> w;
		smej[u].push_back(make_pair(w, v));
		graph[u][v] = w;
	}

	
	//deikstra
		ofstream ans1("deikstra.txt");
		dekstra(1);
		for (int i = 1; i <= v; ++i) {
			if (dist[i] == INF) {
				cout << "INF" << endl;
				continue;
			}
			cout << "x" << i << " = " << dist[i] << " path:";
			ans1 << "x" << i << " = " << dist[i] << " path:";
			int cur = i;
			while (cur != -1) {
				cout << "x" << cur;
				ans1 << "x" << cur;
				cur = parent[cur];
				if (cur != -1) {
					cout << " <- ";
					ans1 << " <- ";
				}
			}
			cout << endl;
			ans1 << endl;
		}
		ans1.close();
		
		
	//orshal
		ofstream ans2("orshala.txt");
		for (int k = 1; k <= v; k++) {
		int	i = 1;
				for (int j = 1; j <= v; j++) {
					if (graph[i][j] > graph[i][k] + graph[k][j]) {
						graph[i][j] = graph[i][k] + graph[k][j];
					}
				}
		}
		int i = 1;
			for (int j = 1; j <= v; j++) {
				if (i == j) continue;
				cout << i << " -> " << j << " = " << graph[i][j] << " path:";
				ans2 << i << " -> " << j << " = " << graph[i][j] << " path:";
				int cur = j;
				while (cur != -1) {
					cout << "x" << cur;
					ans2 << "x" << cur;
					cur = parent[cur];
					if (cur != -1) {
						cout << " <- ";
						ans2 << " <- ";
					}
				}
				cout << endl;
				ans2 << endl;
			}
}
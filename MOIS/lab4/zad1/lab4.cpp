#include <iostream>
#include <vector>
#include <fstream>
#include <vector>

using namespace std;

void find_cycle(vector<int> subst) {
	ofstream out("output.txt", ios::app);
	bool* check = new bool[subst.size()] {0};
	int j = 1;
	for (int i = 0; i < subst.size(); i++) {
		if (!check[i]) {
			out << "cucle " << j << ":(" << i + 1;
			int pos = subst[i];
			check[i] = 1;
			while (pos != i) {
				out << pos + 1;
				check[pos] = 1;
				pos = subst[pos];
				check[pos] = 1;
			}
			out << ")" << endl;
			j++;
		}
	}
}

void find_degree(vector<int> subst) {
	ofstream out("output.txt", ios::app);
	out << "degree of substitution:" << subst.size() << endl;
}

void invers_chet(vector<int> subst) {
	ofstream out("output.txt", ios::app);
	int inv = 0;
	for (int i = 0; i < subst.size()-1; i++) {
		for (int j = i + 1; j < subst.size(); j++) {
			if (subst[i] > subst[j]) inv++;
		}
	}
	out << "parity of substitution by inversions: ";
	if (inv % 2 == 0) out << "true\n"; else out << "false\n";
}

void transpos_chet(const vector<int> subst) {
	ofstream out("output.txt", ios::app);
	int n = subst.size();
	vector<bool> visited(n, false);
	int transpositions = 0;

	for (int i = 0; i < n; i++) {
		if (!visited[i]) {
			int j = i;
			int cycle_size = 0;
			while (!visited[j]) {
				visited[j] = true;
				j = subst[j];
				cycle_size++;
			}
			transpositions += cycle_size - 1;
		}
	}
	out << "parity of substitution by transpositions: ";
	if (transpositions % 2 == 0) out << "true\n"; else out << "false\n";
}


int main() {
	ofstream out("output.txt");
	ifstream in("input.txt");
	vector<int> subst;
	while (!in.eof()) {
		int buf;
		in >> buf;
		subst.push_back(buf-1);
	}
	find_cycle(subst);
	find_degree(subst);
	invers_chet(subst);
	transpos_chet(subst);
}
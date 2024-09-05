#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;
using namespace chrono;
int tryToPlace(int x, int y, vector<vector<int>> board, const int n) {
	int count;
	if (board[x][y] == 0) {
		 count = 1;
	}
	else {
		 count = 0;
	}
	int temp = board[x][y];
	board[x][y] = 2;
	for (int i = 0; i < n; i++) {
		if (board[i][y] == 0) count++;
		if (board[x][i] == 0) count++;
	}
	// Подсчет клеток по диагонали слева вверх
	for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
		if (board[i][j] == 0) count++;
	}

	// Подсчет клеток по диагонали слева вниз
	for (int i = x + 1, j = y - 1; i < n && j >= 0; i++, j--) {
		if (board[i][j] == 0) count++;
	}

	// Подсчет клеток по диагонали справа вверх
	for (int i = x - 1, j = y + 1; i >= 0 && j < n; i--, j++) {
		if (board[i][j] == 0) count++;
	}

	// Подсчет клеток по диагонали справа вниз
	for (int i = x + 1, j = y + 1; i < n && j < n; i++, j++) {
		if (board[i][j] == 0) count++;
	}
	board[x][y] = temp;
	return count;
}

void place(int x, int y, vector<vector<int>>& board, const int n) {
	board[x][y] = 2;
	for (int i = 0; i < n; i++) {
		if (board[i][y] == 0) board[i][y] = 1;
		if (board[x][i] == 0) board[x][i] = 1;
	}
	// Подсчет клеток по диагонали слева вверх
	for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
		if (board[i][j] == 0) board[i][j] = 1;
	}

	// Подсчет клеток по диагонали слева вниз
	for (int i = x + 1, j = y - 1; i < n && j >= 0; i++, j--) {
		if (board[i][j] == 0) board[i][j] = 1;
	}

	// Подсчет клеток по диагонали справа вверх
	for (int i = x - 1, j = y + 1; i >= 0 && j < n; i--, j++) {
		if (board[i][j] == 0) board[i][j] = 1;
	}

	// Подсчет клеток по диагонали справа вниз
	for (int i = x + 1, j = y + 1; i < n && j < n; i++, j++) {
		if (board[i][j] == 0) board[i][j] = 1;
	}
}

void findpositions(int n) {
	vector<vector<int>> board(n, vector<int>(n, 0));

	int count = INT_MAX;
	int x, y;
	int last = n * n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] = 0;
		}
	}

	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (board[i][j] != 2) {
					int count1 = tryToPlace(i, j, board, n);
					if (count1 < count) {
						count = count1;
						x = i;
						y = j;
					}
				}
			}
		}
		place(x, y, board, n);
		last -= count;
		count = INT_MAX;

	}
	cout <<"Free cells: "<< last << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	ofstream out("output.txt");
	for (int n = 1; n < 200; n++) {
		auto start_time = high_resolution_clock::now();
		findpositions(n);
		auto end_time = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(end_time - start_time);
		out << n <<" " << duration.count() << endl;
		cout << "Total time: " << duration.count() << " milliseconds" << endl;
	}
}
	
	
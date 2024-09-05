#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include "matrix.h"
using namespace std;

string padding(string word, int n_bits) {
	while (word.size() % n_bits != 0) {
		word += '.';
	}
	return word;
}

vector<int> XOR(vector<int> a, vector<int> b) {
	vector<int> result(a.size());
	for (int i = 0; i < a.size(); i++) {
		result[i] = (a[i] ^ b[i])%256;
	}
	return result;
}



vector<int> matrixshifr(vector<int> block, vector<int> key,int n_bits) {
	int pos = 0;
	vector<vector<int>> m_block(sqrt(n_bits),vector<int>(sqrt(n_bits)));
	vector<vector<int>> m_key(sqrt(n_bits), vector<int>(sqrt(n_bits)));

	for (int i = 0; i < sqrt(n_bits); i++) {
		for (int j = 0; j < sqrt(n_bits); j++) {
			m_block[i][j] = block[pos];
			m_key[i][j] = key[pos];
			pos++;
		}
	}

	vector<vector<int>> m_result(sqrt(n_bits), vector<int>(sqrt(n_bits)));
	for (int i = 0; i < sqrt(n_bits); ++i) {
		for (int j = 0; j < sqrt(n_bits); ++j) {
			for (int k = 0; k < sqrt(n_bits); ++k) {
				m_result[i][j] += m_key[i][k] * m_block[k][j];
			}
		}
	}
	vector<int> result;
	for (const auto& row : m_result) {
		for (int val : row) {
			result.push_back(val);
		}
	}
	/*cout << "cipher block\n";
	for (const auto& c : result) {
		cout << c << " ";
	}
	cout << '\n';*/
	return result;
}


vector<int> unmatrixshifr(vector<int> cipherblock, vector<int> key, int n_bits) {
	int pos = 0;
	vector<vector<int>> m_cipherblock(sqrt(n_bits), vector<int>(sqrt(n_bits)));
	vector<vector<double>> m_key(sqrt(n_bits), vector<double>(sqrt(n_bits)));

	for (int i = 0; i < sqrt(n_bits); i++) {
		for (int j = 0; j < sqrt(n_bits); j++) {
			m_cipherblock[i][j] = cipherblock[pos];
			m_key[i][j] = key[pos];
			pos++;
		}
	}

	m_key = inverseMatrix(m_key);

	vector<vector<int>> m_result(sqrt(n_bits), vector<int>(sqrt(n_bits)));
	for (int i = 0; i < sqrt(n_bits); ++i) {
		for (int j = 0; j < sqrt(n_bits); ++j) {
			for (int k = 0; k < sqrt(n_bits); ++k) {
				m_result[i][j] += m_key[i][k] * m_cipherblock[k][j];
			}
		}
	}

	vector<int> result;
	for (const auto& row : m_result) {
		for (int val : row) {
			result.push_back(val);
		}
	}
	/*cout << "uncipher block\n";
	for (const auto& c : result) {
		cout << c << " ";
	}
	cout << '\n';*/
	return result;

}

vector<int> CBC(string text, vector<int> key, int n_bits) {
	text = padding(text, n_bits);
	vector<int> iv = key;
	vector<int> ciphertext;
	for (int iter = 0; iter < text.size(); iter += n_bits) {
		vector<int> block;
		for (int i = iter; i < iter+n_bits; ++i) {
			block.push_back(text[i]);
		}
		block = XOR(block, iv);

		//тут будет функция шифрования
		vector<int> cipherblock = matrixshifr(block,key,n_bits);
		
		ciphertext.insert(ciphertext.end(), cipherblock.begin(), cipherblock.end());
		iv = cipherblock;

	}


	/*for (auto c : ciphertext) {
		cout << c << " ";
	}
	cout << endl;*/

	return ciphertext;
}

string unCBC(vector<int> ciphertext, vector<int> key, int n_bits) {
	vector<int> iv = key;
	vector<int> text;
	for (int iter = ciphertext.size()-1; iter > 0; iter-=n_bits) {
		vector<int> block;
		for (int i = iter - n_bits+1; i < iter+1; ++i) {
			block.push_back(ciphertext[i]);
		}
		vector<int> prevblock;
		if (iter > n_bits) {
			for (int i = iter - 2 * n_bits+1; i < iter - n_bits+1; ++i) {
				prevblock.push_back(ciphertext[i]);
			}
		}
		else {
			prevblock = iv;
		}
		//тут должна быть функция расшифра
		block = unmatrixshifr(block,key,n_bits);
		vector<int> decryptedtext = XOR(block,prevblock);
		text.insert(text.begin(), decryptedtext.begin(), decryptedtext.end());
	}
	string result = "";
	for (auto c : text) {
		result += c;
	}
	return result;
}
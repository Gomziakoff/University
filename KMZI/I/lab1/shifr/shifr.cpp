#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>

using namespace std;

pair<pair<map<char, vector<int>>, map<char, vector<int>>>, vector<int>> encrypt_key(string open_text, string fake_text) {
	vector<int> nums;
	
	while (open_text.size() > fake_text.size()) {
		fake_text += " ";
	}
	
	int index = 0;
	for (int i = 0; i < 1000; i++) {
		nums.push_back(i);
	}
	map<char, vector<int>> key; // Исправлено на map<char, vector<int>>
	map<char, vector<int>> fake_key;
	vector<int> text;
	for (int i = 0; i < open_text.size(); i++) {
		index = rand() % nums.size();
		key[open_text[i]].push_back(nums[index]);
		fake_key[fake_text[i]].push_back(nums[index]);
		text.push_back(nums[index]);
		nums.erase(nums.begin() + index);
	}
	return make_pair(make_pair(key, fake_key), text);
}

string decrypt_key(const map<char, vector<int>>& key, const vector<int>& encrypt_text) {
	string open_text = "";
	for (int i = 0; i < encrypt_text.size(); i++) {
		for (auto it = key.begin(); it != key.end(); ++it) {
			for (int j = 0; j < it->second.size(); ++j) {
				if (encrypt_text[i] == it->second[j]) {
					open_text += it->first;
					break;
				}
			}
		}
	}
	return open_text;
}



int main()
{
	string open_text, fake_text;
	cout << "enter text to encrypt\n";
	getline(cin, open_text);
	cout << "enter fake text\n";
	getline(cin, fake_text);
	pair<pair<map<char, vector<int>>, map<char, vector<int>>>, vector<int>> crypt_pair = encrypt_key(open_text,fake_text);
	map<char, vector<int>> key = crypt_pair.first.first;
	map<char, vector<int>> fake_key = crypt_pair.first.second;
	vector<int> encrypt_text = crypt_pair.second;

	// Вывод encrypt_text
	cout << "Encrypted text:" << endl;
	for (int i = 0; i < encrypt_text.size(); ++i) {
		cout << encrypt_text[i] << " ";
	}
	cout << endl;

	// Вывод key
	cout << "Key:" << endl;
	for (auto it = key.begin(); it != key.end(); ++it) {
		cout << it->first << ": ";
		for (int i = 0; i < it->second.size(); ++i) {
			cout << it->second[i] << " ";
		}
		cout << endl;
	}

	// Вывод fake_key
	cout << "Fake_key:" << endl;
	for (auto it = fake_key.begin(); it != fake_key.end(); ++it) {
		cout << it->first << ": ";
		for (int i = 0; i < it->second.size(); ++i) {
			cout << it->second[i] << " ";
		}
		cout << endl;
	}

	string decrypted_text = decrypt_key(key, encrypt_text);
	string fake_decrypted_text = decrypt_key(fake_key, encrypt_text);
	cout << "Decrypted text: " << decrypted_text << endl;
	cout << "Fake decrypted text: " << fake_decrypted_text << endl;
}

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <random>
#include <windows.h>
#include <locale>
#include <string>
using namespace std;

class ElipticCurves {
	int p = 751;
	int a = -1;
	int b = 1;
	pair<int, int> G = { 0,1 };
	pair<int, int> Gq23 = { -1,1 };
	pair<int, int> G5 = { 416,55 };
	int n = 13;
	pair<int, int> G6 = { 562,89 };
	pair<int, int> GTest = { 384,475 };

	map<int, pair<int, int>> alphabet = {
		{' ', {33, 355}},
		{'!', {33, 396}},
		{'"', {34, 74}},
		{'#', {34, 677}},
		{'$', {36, 87}},
		{'%', {36, 664}},
		{'&', {39, 171}},
		//{"'", {39, 171}},
		{'(', {43, 224}},
		{')', {43, 527}},
		{'*', {44, 366}},
		{'+', {44, 385}},
		{',', {45, 31}},
		{'-', {45, 720}},
		{'.', {47, 349}},
		{'/', {47, 402}},
		{'0', {48, 49}},
		{'1', {48, 702}},
		{'2', {49, 183}},
		{'3', {49, 568}},
		{'4', {53, 227}},
		{'5', {53, 474}},
		{'6', {56, 332}},
		{'7', {56, 419}},
		{'8', {58, 139}},
		{'9', {58, 612}},
		{':', {59, 365}},
		{';', {59, 386}},
		{'<', {61, 129}},
		{'=', {61, 622}},
		{'>', {62, 372}},
		{'?', {62, 379}},
		{'@', {66, 199}},
		{'A', {66, 552}},
		{'B', {67, 84}},
		{'C', {67, 667}},
		{'D', {69, 241}},
		{'E', {69, 510}},
		{'F', {70, 195}},
		{'G', {70, 556}},
		{'H', {72, 254}},
		{'I', {72, 497}},
		{'J', {73, 72}},
		{'K', {73, 679}},
		{'L', {74, 170}},
		{'M', {74, 581}},
		{'N', {75, 318}},
		{'O', {75, 433}},
		{'P', {78, 271}},
		{'Q', {78, 480}},
		{'R', {79, 111}},
		{'S', {79, 640}},
		{'T', {80, 318}},
		{'U', {80, 433}},
		{'V', {82, 270}},
		{'W', {82, 481}},
		{'X', {83, 373}},
		{'Y', {83, 378}},
		{'Z', {85, 35}},
		{'[', {85, 716}},
		//{'\', {86, 25}},
		{']', {86, 726}},
		{'^', {90, 21}},
		{'_', {90, 730}},
		{'`', {93, 267}},
		{'a', {93, 484}},
		{'b', {98, 338}},
		{'c', {98, 413}},
		{'d', {99, 295}},
		{'e', {99, 456}},
		{'f', {100, 364}},
		{'g', {100, 387}},
		{'h', {102, 267}},
		{'i', {102, 484}},
		{'j', {105, 369}},
		{'k', {105, 382}},
		{'l', {106, 24}},
		{'m', {106, 727}},
		{'n', {108, 247}},
		{'o', {108, 504}},
		{'p', {109, 200}},
		{'q', {109, 551}},
		{'r', {110, 129}},
		{'s', {110, 622}},
		{'t', {114, 144}},
		{'u', {114, 607}},
		{'v', {115, 242}},
		{'w', {115, 509}},
		{'x', {116, 92}},
		{'y', {116, 659}},
		{'z', {120, 147}},
		{'{', {120, 604}},
		{'|', {125, 292}},
		{'}', {125, 459}},
		{'~', {126, 33}},
		{'А', {189, 297}},
		{'Б', {189, 458}},
		{'В', {192, 32}},
		{'Г', {192, 719}},
		{'Д', {194, 205}},
		{'Е', {194, 546}},
		{'Ж', {197, 145}},
		{'З', {197, 606}},
		{'И', {198, 224}},
		{'Й', {198, 527}},
		{'К', {200, 30}},
		{'Л', {200, 721}},
		{'М', {203, 324}},
		{'Н', {203, 427}},
		{'О', {205, 372}},
		{'П', {205, 379}},
		{'Р', {206, 106}},
		{'С', {206, 645}},
		{'Т', {209, 82}},
		{'У', {209, 669}},
		{'Ф', {210, 31}},
		{'Х', {210, 720}},
		{'Ц', {215, 247}},
		{'Ч', {215, 504}},
		{'Ш', {218, 150}},
		{'Щ', {218, 601}},
		{'Ъ', {221, 138}},
		{'Ы', {221, 613}},
		{'Ь', {226, 9}},
		{'Э', {226, 742}},
		{'Ю', {227, 299}},
		{'Я', {227, 452}},
		{'а', {228, 271}},
		{'б', {228, 480}},
		{'в', {229, 151}},
		{'г', {229, 600}},
		{'д', {234, 164}},
		{'е', {234, 587}},
		{'ж', {235, 19}},
		{'з', {235, 732}},
		{'и', {236, 39}},
		{'й', {236, 712}},
		{'к', {237, 297}},
		{'л', {237, 454}},
		{'м', {238, 175}},
		{'н', {238, 576}},
		{'о', {240, 309}},
		{'п', {240, 442}},
		{'р', {243, 87}},
		{'с', {243, 664}},
		{'т', {247, 266}},
		{'у', {247, 485}},
		{'ф', {249, 183}},
		{'х', {249, 568}},
		{'ц', {250, 14}},
		{'ч', {250, 737}},
		{'ш', {251, 245}},
		{'щ', {251, 506}},
		{'ъ', {253, 211}},
		{'ы', {253, 540}},
		{'ь', {256, 121}},
		{'э', {256, 630}},
		{'ю', {257, 293}},
		{'я', {257, 458}}
	};
public:
	int positiveMod(int a, int m) {
		return (a % m + m) % m;
	}

	int extendEucled(int a, int b, int &x, int &y) {
		if (a == 0) {
			x = 0;
			y = 1;
			return b;
		}
		int x1, y1;
		int gcd = extendEucled(positiveMod(b,a), a, x1, y1);
		x = y1 - (b / a) * x1;
		y = x1;
		return gcd;
	}

	int gcd(int a, int m) {
		if (a == 0) {
			return 0;
		}
		int gcd, x, y;
		gcd = extendEucled(a, m, x, y);
		if (gcd != 1) {
			cout << "Error gcd\n";
			return 0;
		}
		return positiveMod(x, m);
	}

	pair<int, int> doubleAdditing(pair<int, int> P, pair<int, int> Q) {
		if (P.first == -1) return Q;//maybe error
		if (Q.first == -1) return P;
		int x1 = P.first, y1 = P.second;
		int x2 = Q.first, y2 = Q.second;

		int lm;
		if (P != Q) {
			int denom1 = positiveMod((y2 - y1), p);
			int denom2 = positiveMod((x2 - x1), p);
			denom2 = gcd(denom2, p);
			lm = positiveMod((denom1 * denom2), p);
		}
		else {
			int denom1 = positiveMod((3 * x1 * x1 + a), p);
			int denom2 = positiveMod((2 * y1), p);
			denom2 = gcd(denom2, p);
			lm = positiveMod((denom1 * denom2), p);
		}

		int x3 = positiveMod((lm * lm - x1 - x2), p);
		int y3 = positiveMod((lm * (x1 - x3) - y1), p);
		return { x3,y3 };
	}

	int bit_length(unsigned int a) {
		return a > 0 ? static_cast<int>(std::log2(a)) + 1 : 0;
	}

	pair<int, int> multiplyPoint(int k, pair<int, int> P) {
		pair<int, int> Q = {-1,-1};
		for (int i = 0; i < bit_length(k); i++) {
			if ((k >> i) & 1) {
				Q = doubleAdditing(P, Q);
			}
			P = doubleAdditing(P, P);
		}
		return Q;
	}

	pair<pair<int, int>, pair<int, int>> encryptChar(int k, pair<int, int> Pb, char _char) {
		pair<int, int> C1 = multiplyPoint(k, G);
		pair<int, int> C2 = multiplyPoint(k, Pb);
		C2 = doubleAdditing(C2, alphabet[_char]);
		return { C1,C2 };
	}

	void encryptText(vector<int> k, pair<int,int> Pb, string plaintext) {
		vector<pair<pair<int, int>, pair<int, int>>> answer;
		for (int i = 0; i < plaintext.size(); i++) {
			answer.push_back(encryptChar(k[i], Pb, plaintext[i]));
			cout <<"x: (" << answer[i].first.first << "," << answer[i].first.second << ")k * G, y: (" << answer[i].second.first << "," << answer[i].second.second << ")\n";
		}
		cout << "\n";
	}

	pair<int, int> reverseSign(pair<int, int> C) {
		int x = C.first;
		int y = -1 * C.second;
		return {x,y};
	}

	int findKeyByValue(const std::pair<int, int>& value) {
		for (const auto& pair : alphabet) {
			if (pair.second == value) {
				return pair.first;
			}
		}
		return '\0'; 
	}

	wchar_t decryptChar(pair<pair<int, int>, pair<int, int>> C, int nb) {
		pair<int, int> C_ = multiplyPoint(nb, C.first);
		pair<int, int> C_m = reverseSign(C_);
		pair<int, int> C_a = doubleAdditing(C.second, C_m);
		int char_ = findKeyByValue(C_a);
		if (!char_) {
			wcout << L"Error char index\n";
			return 0;
		}
		return static_cast<wchar_t>(char_);
	}

	wstring decryptText(int nb, vector<pair<pair<int, int>, pair<int, int>>> ciphertext) {
		wstring decryptedText = L"";
		for (auto C : ciphertext) {
			wchar_t char_ = decryptChar(C, nb);
			if (!char_) {
				wcout << L"error find char";
				return L"";
			}
			decryptedText += char_;
		}
		wcout << decryptedText;
		return decryptedText;
	}

	pair<int, int> signatureGenerate(int k, int e, int d) {
		pair<int, int> kG = multiplyPoint(k, G5);
		int r = positiveMod(kG.first, n);
		int z = gcd(k, n);
		int s = positiveMod(z * (e + d * r), n);
		cout << "r: " << r << " s: " << s << endl;
		return { r,s };
	}

	bool signatureVerification(int e, pair<int, int> rs, pair<int, int> Q) {
		if (1 <= rs.first <= n - 1 && 1 <= rs.second <= n - 1) {
			int v = gcd(rs.second, n);
			int u1 = positiveMod(e * v, n);
			int u2 = positiveMod(rs.first * 3, n);
			pair<int, int> u1G = multiplyPoint(u1, G6);
			pair<int, int> u2Q = multiplyPoint(u2, Q);
			pair<int, int> X = doubleAdditing(u1G, u2Q);
			int X_ = positiveMod(X.first, n);
			if (rs.first == X_) {
				cout << "r: " << rs.first << " x: " << X_ << endl;
				return 1;
			}
			else {
				cout << "r: " << rs.first << " x: " << X_ << endl;
				return 0;
			}
		}
		else {
			cout << "error sign ver";
			return -1;
		}
	}
};

class Task1 {
public:
	string plaintext;
	pair<int, int> Pb;
	vector<int> k;

	Task1(string plaintext, pair<int, int> Pb, vector<int> k) {
		this->plaintext = plaintext;
		this->Pb = Pb;
		this->k = k;
	}

	void activate(ElipticCurves obj) {
		obj.encryptText(k, Pb, plaintext);
	}
};

class Task2 {
	int nb;
	vector<pair<pair<int, int>, pair<int, int>>> ciphertext;
public:
	Task2(int nb, vector<pair<pair<int, int>, pair<int, int>>> ciphertext) {
		this->nb = nb;
		this->ciphertext = ciphertext;
	}

	void activate(ElipticCurves obj) {
		obj.decryptText(nb,ciphertext);
	}

};

class Task3 {
public:
	pair<int,int> P;
	pair<int, int> Q;
	pair<int, int> R;
	int kP = 2;
	int kQ = 3;
	Task3(pair<int, int> P, pair<int, int> Q, pair<int, int> R) {
		this->P = P;
		this->Q = Q;
		this->R = R;
	}

	void activate(ElipticCurves obj) {
		pair<int, int> p2 = obj.multiplyPoint(kP, P);
		pair<int, int> q3 = obj.multiplyPoint(kQ, Q);
		pair<int, int> r_ = obj.reverseSign(R);
		pair<int, int> p2q3 = obj.doubleAdditing(p2, q3);
		pair<int, int> C = obj.doubleAdditing(p2q3, r_);
		cout << "x: " << C.first << " y: " << C.second << endl;
	}
};

class Task4 {
public:
	pair<int, int> P;
	int k;
	Task4(pair<int, int> P, int k) {
		this->P = P;
		this->k = k;
	}

	void activate(ElipticCurves obj) {
		pair<int, int> C = obj.multiplyPoint(k, P);
		cout << "x: " << C.first << " y: " << C.second << endl;
	}
};

class Task5 {
	int e;
	int d;
	int k;
public:
	Task5(int e, int d, int k) {
		this->e = e;
		this->d = d;
		this->k = k;
	}
	void activate(ElipticCurves obj) {
		obj.signatureGenerate(k, e, d);
	}
};

class Task6 {
	int e;
	pair<int, int> Q;
	pair<int, int> rs;
public:
	Task6(int e,pair<int, int> Q,pair<int, int> rs) {
		this->e = e;
		this->Q = Q;
		this->rs = rs;
	}
	void activate(ElipticCurves obj) {
		obj.signatureVerification(e, rs, Q);
	}
};




int main() {

	ElipticCurves obj;
	cout << "Task 1\n";
	Task1 task1("Renessans", { 725,195 }, vector<int>{2, 19, 4, 8, 2, 2, 16, 10, 2});
	task1.activate(obj);
	cout << "Task 2\n";
	vector<pair<pair<int, int>, pair<int, int>>> data = {
		{{188, 93}, {623, 166}},
		{{725, 195}, {513, 414}},
		{{346, 242}, {461, 4}},
		{{489, 468}, {739, 574}},
		{{725, 195}, {663, 476}},
		{{745, 210}, {724, 522}},
		{{725, 195}, {663, 476}},
		{{618, 206}, {438, 40}},
		{{286, 136}, {546, 670}},
		{{179, 275}, {73, 72}}
	};
	Task2 task2(32, data);
	task2.activate(obj);
	cout << "Task 3\n";
	Task3 task3({ 59,386 }, { 61,129 }, { 100,364 });
	task3.activate(obj);
	cout << "Task 4\n";
	Task4 task4({ 45,720 },111);
	task4.activate(obj);
	cout << "Task 5\n";
	Task5 task5(11,5,6);
	task5.activate(obj);
	cout << "Task 6\n";
	Task6 task6(7, {384,475}, { 5,5 });
	task6.activate(obj);
}
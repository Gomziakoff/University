#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cassert>

using namespace std;

const int r1[64] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
	3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
	1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
};
const int r2[64] = {
	5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
	6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
	15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
	8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14
};
const int s1[64] = {
	11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
	7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
	11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
	11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12
};
const int s2[64] = {
	8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
	9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
	9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
	15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8
};
uint32_t K1(unsigned int j) {
	return
		j < 16 ? 0x00000000 :
		j < 32 ? 0x5A827999 :
		j < 48 ? 0x6ED9EBA1 :
		0x8F1BBCDC;
}
uint32_t K2(unsigned int j) {
	return
		j < 16 ? 0x50A28BE6 :
		j < 32 ? 0x5C4DD124 :
		j < 48 ? 0x6D703EF3 :
		0x00000000;
}
uint32_t f(uint32_t x, uint32_t y, uint32_t z, unsigned int round) {
	if (round < 16) return (x ^ y ^ z);
	if (round < 32) return ((x & y) | (~x & z));
	if (round < 48) return ((x | ~y) ^ z);
	return ((x & z) | (y & ~z));
}
void ripemd256(const uint8_t* input, size_t length, uint8_t* output) {
	uint32_t h[8] = { 0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,
			0x76543210,0xFEDCBA98,0x89ABCDEF,0x01234567 };
	vector<uint8_t> padded_data(length + 64);
	memcpy(padded_data.data(), input, length);
	padded_data[length] = 0x80;
	size_t padded_length = length + 1;
	while (padded_length % 64 != 56) {
		padded_data[padded_length++] = 0x00;
	}
	uint64_t bit_length = length * 8;
	padded_data[padded_length++] = bit_length & 0xFF;
	padded_data[padded_length++] = (bit_length >> 8) & 0xFF;
	padded_data[padded_length++] = (bit_length >> 16) & 0xFF;
	padded_data[padded_length++] = (bit_length >> 24) & 0xFF;
	padded_data[padded_length++] = (bit_length >> 32) & 0xFF;
	padded_data[padded_length++] = (bit_length >> 40) & 0xFF;
	padded_data[padded_length++] = (bit_length >> 48) & 0xFF;
	padded_data[padded_length++] = (bit_length >> 56) & 0xFF;
	for (size_t i = 0; i < padded_length; i += 64) {
		uint32_t w[16];
		for (int j = 0; j < 16; ++j) {
			w[j] = padded_data[i + j * 4 + 0] |
				(padded_data[i + j * 4 + 1] << 8) |
				(padded_data[i + j * 4 + 2] << 16) |
				(padded_data[i + j * 4 + 3] << 24);
		}
		uint32_t A1 = h[0];
		uint32_t B1 = h[1];
		uint32_t C1 = h[2];
		uint32_t D1 = h[3];
		uint32_t A2 = h[4];
		uint32_t B2 = h[5];
		uint32_t C2 = h[6];
		uint32_t D2 = h[7];
		uint32_t T;
		for (int j = 0; j < 64; j++) {
			T = (A1 + f(B1, C1, D1, j) + w[r1[j]] + K1(j)) & 0xFFFFFFFF;
			T = (T << s1[j]) | (T >> (32 - s1[j]));  // Rotate left by s1[j]
			A1 = D1;
			D1 = C1;
			C1 = B1;
			B1 = T;
			T = (A2 + f(B2, C2, D2, 63 - j) + w[r2[j]] + K2(j)) & 0xFFFFFFFF;
			T = (T << s2[j]) | (T >> (32 - s2[j]));  // Rotate left by s2[j]
			A2 = D2;
			D2 = C2;
			C2 = B2;
			B2 = T;
			if (j == 15) { std::swap(A1, A2); }
			if (j == 31) { std::swap(B1, B2); }
			if (j == 47) { std::swap(C1, C2); }
			if (j == 63) { std::swap(D1, D2); }
		}
		h[0] = h[0] + A1;
		h[1] = h[1] + B1;
		h[2] = h[2] + C1;
		h[3] = h[3] + D1;
		h[4] = h[4] + A2;
		h[5] = h[5] + B2;
		h[6] = h[6] + C2;
		h[7] = h[7] + D2;
	}
	memcpy(output, h, 32);
}
int main() {
	string input = "Hello world";
	uint8_t hash[32];
	ripemd256(reinterpret_cast<const uint8_t*>(input.c_str()), input.size(), hash);
	cout << "RIPEMD-256 hash: ";
	for (size_t i = 0; i < sizeof(hash); ++i) {
		cout << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
	}
	cout << endl;
	return 0;
}
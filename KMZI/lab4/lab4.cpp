#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include "ap/ap.hpp"

ap_uint<32768> long_pow(ap_uint<32768>a, ap_uint<32768>b) {
    ap_uint<32768> result = 1;
    for (int i = 0; i < b-1; i++) {
        result *= a;
    }
    return result;
}

ap_uint<32768> modulo(ap_uint<32768>a, ap_uint<32768>b) {
    if (a < b) {
        return a;
    }
    else {
        return a - (a / b) * b;
    }
}


int true_rand_num(int p = 8) {
    unsigned int random_number = rand() % (1 << p); // Генерируем случайное число от 0 до 2^p - 1
    random_number |= (1u << (p - 1));  // Устанавливаем старший бит в 1
    random_number |= 1u;           // Устанавливаем младший бит в 1
    return random_number;
}

int NOD(int a,int b) {
    if (a % b == 0) {
        return b;
    }
    if (b % a == 0) {
        return a;
    }

    if (a > b) {
        return NOD(a % b, b);
    }
    return NOD(a, a % b);
}

std::vector<int> sieve_of_eratosthen(int n) {
    std::vector<bool> is_prime(n + 1, true);
    std::vector<int> primes;

    is_prime[0] = is_prime[1] = false; // 0 и 1 не являются простыми числами

    for (int i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}

bool bruteforce_check(int n, std::vector<int> primes) {
    for (int prime : primes) {
        if (prime >= n) break;
        if (n % prime == 0) return 0;
    }
    return 1;
}

bool rubber_miller_test(int n) {
    int t = n - 1;
    int s = 0;
    while (t % 2 == 0) {
        t /= 2;
        s += 1;
    }
    
    for (int k = 0; k < 5; k++) {
        int a = rand() % (n - 3) + 2;
        ap_uint<32768> x = modulo(long_pow(a, t),n);
        if (x == 1 || x == n - 1) continue;
        for (int b = 0; b < s - 1; b++) {
            x = x * x % n;
            if (x == 1) return 0;
            if (x == n - 1) break;
        }
        if (x != n - 1) return 0;
    }
    return 1;
}

int main() {
    srand(time(nullptr));
    int p = 32; //требуемая длина простого числа
    int n;
    std::vector<int> primes = sieve_of_eratosthen(2000);

    while (true) {
        n = true_rand_num(p);
        if (!bruteforce_check(n, primes)) continue;
        if (rubber_miller_test(n)) break;
    }

    std::cout << "простое число: " << n;
    

    return 0;
}


#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include "ap/ap.hpp"

ap_uint<32768> long_pow(ap_uint<32768>a, ap_uint<32768>b) {
    ap_uint<32768> result = 1;
    for (int i = 0; i < b - 1; i++) {
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

int NOD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
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

bool rubber_miller_test(int n) {
    if (n <= 3) return 1;
    int t = n - 1;
    int s = 0;
    while (t % 2 == 0) {
        t /= 2;
        s += 1;
    }
    
    for (int k = 0; k < 5; k++) {
        int a = rand() % (n - 3) + 2;
        ap_uint<32768> x = modulo(long_pow(a, t), n);
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

int true_rand_num(int p = 8) {
    while (true)
    {
        unsigned int random_number = rand() % (1 << p) + 4;
        random_number |= 1u;
        if (!rubber_miller_test(random_number)) {
            return random_number;
        }
    }
}


std::vector<int> bruteforce_factorization(int n, std::vector<int> primes) {
    std::vector<int> result;
    for (int num : primes) {
        while (n % num == 0) {
            n /= num;
            result.push_back(num);
        }
    }
    return result;
}

int func(int x, int n) {
    return ((x * x) - x) % n;
}

std::vector<int> factorization(int n) {
    std::vector<int> result;
    int N = n;
    while (n != 1) {
        int x = rand() % (n - 1) + 1; // Изменение диапазона генерации случайного числа
        int y = x;
        int d = 1;

        while (d == 1) {
            x = func(x, n);
            y = func(func(y, n), n);
            d = NOD(abs(x - y), n);
        }

        if (d != N) {
            n /= d;
            if (!rubber_miller_test(d)) { 
                std::vector<int> factors_of_d = factorization(d); // Рекурсивная факторизация непростого делителя
                result.insert(result.end(), factors_of_d.begin(), factors_of_d.end());
            }
            else {
                result.push_back(d);
            }
        }
    }
    return result;
}

int main() {
    srand(time(NULL));
    std::vector<int> primes = sieve_of_eratosthen(256);

    int n = true_rand_num(8);

    std::cout << n << "\nbruteforce factorization\n";
    for (int num : bruteforce_factorization(n,primes)) {
        std::cout << num << " ";
    }

    std::cout << "\nrho-pollard\n";
    for (int num : factorization(n)) {
        std::cout << num << " ";
    }
}
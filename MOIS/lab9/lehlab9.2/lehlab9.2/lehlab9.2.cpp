#include <iostream>
#include <cmath>

using namespace std;

int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++) {
        f *= i;
    }
    return f;
}

int calculateAnk(int n, int k) {
    if (factorial(n - k) > 0)
        return factorial(n) / factorial(n - k); else return -1;
}

pair<int, int> findMinPair(int m) {
    int minSum = INT_MAX;
    std::pair<int, int> minPair = { 0, 0 };

    for (int n = 1; ; ++n) {
        for (int k = 1; k <= n; ++k) {
            int currentSum = calculateAnk(n, k);
            if (currentSum > pow(10, m) && currentSum < minSum) {
                minSum = currentSum;
                minPair = { n, k };
                return minPair;
            } else if (calculateAnk(n, k) == -1) return minPair = { 0, 0 };
        }
    }

    
}

int main() {
    int mValue = 2;  // Задайте нужное значение m
    std::pair<int, int> result = findMinPair(mValue);
    cout <<"for m = " << mValue << " first pair n and k: (" << result.first << ", " << result.second << ")\n";
    return 0;
}

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <map>

class FibonacciRandomGenerator {
private:
    std::vector<double> sequence;
    int a;
    int b;

public:
    FibonacciRandomGenerator(int a, int b) {
        int length = std::max(a, b);
        this->a = a;
        this->b = b;
        for (int i = 0; i < length; i++) {
            double random_number = static_cast<double>(rand()) / RAND_MAX;
            this->sequence.push_back(random_number);
        }
    }

    double Next() {
        if (sequence[sequence.size() - a] >= sequence[sequence.size() - b]) {
            sequence.push_back(sequence[sequence.size() - a] - sequence[sequence.size() - b]);
        }
        else {
            sequence.push_back(sequence[sequence.size() - a] - sequence[sequence.size() - b] + 1);
        }
        return sequence.back();
    }

    std::vector<int> generateSeq(int n, int max) {
        std::vector<int> sequence;
        for (int i = 0; i < n; i++) {
            sequence.push_back(Next() * max);
        }
        return sequence;
    }
};

int main() {
    FibonacciRandomGenerator generator(55, 24);
    std::map<int, int> raspred;
    int max_n = 1000;
    for (int i = 0; i < max_n; i++) {
        raspred[i] = 0;
    }
    for (auto num : generator.generateSeq(10,max_n)) {
        std::cout << num << " ";
        raspred[num]++;
    }
    std::cout << "\n";
    for (auto num : raspred) {
        std::cout << num.first << " " << num.second << "\n";
    }

    return 0;
}

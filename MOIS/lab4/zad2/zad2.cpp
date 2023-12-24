#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <set>

using namespace std;

vector<int> group = { 1, 2, 4, 8 };
vector<vector<int>> subgroups = { {1}, {1, 2}, {1, 4}, {1, 8}, {1, 2, 4, 8} };

set<int> left_coset(int h, const vector<int>& subgroup) {
    set<int> coset;
    for (int g : subgroup) {
        coset.insert((h * g) % 15);
    }
    return coset;
}

void print_set(const set<int>& s) {
    cout << "{ ";
    for (int x : s) {
        cout << x << " ";
    }
    cout << "}";
}

int main() {
    vector<int> A = { 1, 2, 4, 8 };
    vector<vector<int>> AK(A.size(), vector<int>(A.size()));

    cout << setw(2) << " *|";
    for (int j = 0; j < A.size(); j++) {
        cout << setw(2) << A[j] << " ";
    }
    cout << endl;
    cout << " _+___________\n";

    for (int i = 0; i < A.size(); i++) {
        cout << setw(2) << A[i] << "|";
        for (int j = 0; j < A.size(); j++) {
            AK[i][j] = A[i] * A[j] % 15; 
            cout << setw(2) << AK[i][j] << " ";
        }
        cout << endl;
    }

    cout << "neytrall element: 1\n";
    cout << "inverse elements:\n";
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A.size(); j++) {
            if (AK[i][j] == 1) {
                cout << A[i] << ":" << A[j] << endl;
            }
        }
    }

    cout << "Groups:\n";
    vector<vector<int>> groups(A.size(), vector<int>(A.size()));
    bool check = 0;
    for (int i = 0; i < A.size(); i++) {
        int count = 0;
        cout << A[i] << ":{ ";
        int k = A[i];
         do {
            k = A[i]*k%15;
            groups[i][count] = k;
            cout << k << " ";
            count++;
         } while (k != A[i]);
        cout << "} order of an element: " << count << endl;
        if (count == A.size()) check = 1;
    }
    cout << "Group is cyclic: ";
    if (check) cout << "TRUE\n"; else cout << "FALSE\n";

    cout << setw(2) << " *|";
    for (int j = 0; j < 2; j++) {
        cout << setw(2) << groups[2][j] << " ";
    }
    cout << endl;
    cout << " _+___________\n";
    for (int i = 0; i < 2; i++) {
        cout << setw(2) << groups[2][i] << "|";
        for (int j = 0; j < 2; j++) {
            cout << setw(2) << groups[2][i]* groups[2][j]%15 << " ";
        }
        cout << endl;
    }
    for (const vector<int>& H : subgroups) {
        cout << "Subgroup: ";
        print_set(set<int>(H.begin(), H.end()));
        cout << "\nLeft cosets:\n";
        set<set<int>> cosets;
        for (int h : group) {
            set<int> coset = left_coset(h, H);
            if (!cosets.count(coset)) {
                cosets.insert(coset);
                print_set(coset);
                cout << "\n";
            }
        }
        cout << "Factor set: \n";
        for (const auto& coset : cosets) {
            print_set(coset);
            cout << "\n";
        }
        cout << "\n";

    }
    
    return 0;
}

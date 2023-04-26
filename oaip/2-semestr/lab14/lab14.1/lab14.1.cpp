#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

void printTree(TreeNode* root, string indent, bool last) {
    if (root) {
        cout << indent;
        if (last) {
            cout << "└─";
            indent += "  ";
        }
        else {
            cout << "├─";
            indent += "| ";
        }
        cout << root->val << endl;
        printTree(root->left, indent, root->right == NULL);
        printTree(root->right, indent, true);
    }
}

TreeNode* sortedArrayToBST(vector<int>& nums, int left, int right) {
    if (left > right) {
        return NULL;
    }
    int mid = left + (right - left) / 2;
    TreeNode* root = new TreeNode(nums[mid]);
    root->left = sortedArrayToBST(nums, left, mid - 1);
    root->right = sortedArrayToBST(nums, mid + 1, right);
    return root;
}

double getAvg(TreeNode* root) {
    if (!root) {
        return 0.0;
    }
    double sum = root->val;
    double count = 1.0;
    if (root->left) {
        sum += getAvg(root->left);
        count += 1.0;
    }
    if (root->right) {
        sum += getAvg(root->right);
        count += 1.0;
    }
    return sum / count;
}

int main() {
    int n;
    cout << "введите длину листа: ";
    cin >> n;
    vector<int> nums(n);
    cout << "введите элементы: ";
    for (int i = 0; i < n; i++) {
        cin >> nums[i];
    }
    sort(nums.begin(), nums.end());
    TreeNode* root = sortedArrayToBST(nums, 0, n - 1);
    double avg = getAvg(root);
    cout << "среднее дерева: " << avg << endl;
    printTree(root, "", true);
    return 0;
}

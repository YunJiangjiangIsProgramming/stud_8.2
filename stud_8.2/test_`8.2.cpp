#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include"AVLTree.h"
#include<vector>


using namespace std;

class Solution0 {
public:
    long long numberOfRightTriangles(vector<vector<int>>& grid) {
        int n = grid.size(); // 确保n是网格的列数
        vector<int> col_sum(n, 0); // 初始化为0，避免减1操作

        for (auto& row : grid) {
            for (int j = 0; j < n; j++) {
                col_sum[j] += row[j]; // 计算每列的1的个数
            }
        }

        long long ans = 0;
        for (auto& row : grid) {
            int row_sum = std::count(row.begin(), row.end(), 1); // 使用std::count计算当前行1的个数

            for (size_t j = 0; j < row.size(); j++) { // 使用size_t确保j在合法范围内
                if (row[j] == 1) {
                    // 确保col_sum[j]在合法范围内
                    if (j < col_sum.size()) {
                        ans += row_sum * col_sum[j];
                    }
                }
            }
        }
        return ans;
    }
};


int main()
{
    TestAVLTree1();
	TestAVLTree2();

	return 0;
}

#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include"AVLTree.h"
#include<vector>


using namespace std;

class Solution0 {
public:
    long long numberOfRightTriangles(vector<vector<int>>& grid) {
        int n = grid.size(); // ȷ��n�����������
        vector<int> col_sum(n, 0); // ��ʼ��Ϊ0�������1����

        for (auto& row : grid) {
            for (int j = 0; j < n; j++) {
                col_sum[j] += row[j]; // ����ÿ�е�1�ĸ���
            }
        }

        long long ans = 0;
        for (auto& row : grid) {
            int row_sum = std::count(row.begin(), row.end(), 1); // ʹ��std::count���㵱ǰ��1�ĸ���

            for (size_t j = 0; j < row.size(); j++) { // ʹ��size_tȷ��j�ںϷ���Χ��
                if (row[j] == 1) {
                    // ȷ��col_sum[j]�ںϷ���Χ��
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

#pragma once

#include<assert.h>
#include<time.h>
#include<utility>
#include<iostream>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	std::pair<K, V> _kv;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;//������

	int _bf;

	AVLTreeNode(const std::pair<K, V>& kv)
		: _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0)
	{
		
	}
};

template<class K, class V>
struct AVLTree
{
	typedef AVLTreeNode<K, V> Node;

public:
	bool Insert(const std::pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			return true;
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}
		cur = new Node(kv);
		if (parent->_kv.first < kv.first)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		while (parent)//����bf
		{
			//bf=right-left �������֮��ʼ���²�������������ȵ�bf��blance factor��
			if (cur == parent->_left)
			{
				parent->_bf--;

			}
			else
			{
				parent->_bf++;
			}
			// �Ƿ�����������ݣ������ĸ߶��Ƿ�仯
			// 1��parent->_bf == 0˵��֮ǰparent->_bf�� 1 ���� -1
			// ˵��֮ǰparentһ�߸�һ�ߵͣ���β������ϰ����Ǳߣ�parent���������߶Ȳ��䣬����Ҫ�������ϸ���
			// 2��parent->_bf == 1 �� -1 ˵��֮ǰ��parent->_bf == 0������һ���ߣ����ڲ���һ�߸����ˣ�
			// parent���������߶ȱ��ˣ��������ϸ���
			// 3��parent->_bf == 2 �� -2��˵��֮ǰparent->_bf == 1 ���� -1�����ڲ������ز�ƽ�⣬Υ������
			// �͵ش���--��ת

			// ��ת��
			// 1��������������Ҹ߶Ȳ�����1
			// 2����ת�����м�����������������
			// 3�����µ������ӽڵ��ƽ������
			// 4������������ĸ߶ȸ�����ǰ����һ��

			if (parent->_bf == 0)
			{
				break;
			}
			else if (parent->_bf == 1 || parent->_bf == -1)
			{
				cur = parent;
				parent = parent->_parent;//���ϸ���

			}
			else if (parent->_bf == -2 || parent->_bf == 2)
			{
				//��ʼ��ת����
				// ��ת
				if (parent->_bf == 2 && cur->_bf == 1)
				{
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == -1)
				{
					RotateR(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					RotateRL(parent);
				}
				else
				{
					assert(false);
				}

				break;
			}
			else
			{
				assert(false);
			}
		}
	

		return true;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
		{
			subRL->_parent = parent;
		}

		subR->_left = parent;
		Node* ppNode = parent->_parent;
		parent->_parent = subR;

		if (_root == parent)
		{
			_root = subR;
			subR->_parent = nullptr;

		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subR;
			}
			else
			{
				ppNode->_right = subR;
			}
			subR->_parent = ppNode;
		}
		parent->_bf = subR->_bf = 0;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}

		Node* ppNode = parent->_parent;
		subL->_right = parent;
		parent->_parent = subL;

		//if (_root == parent)
		if (ppNode == nullptr)
		{
			_root = subL;
			_root->_parent = nullptr;
		}
		else
		{
			if (ppNode->_left == parent)
			{
				ppNode->_left = subL;
			}
			else
			{
				ppNode->_right = subL;
			}

			subL->_parent = ppNode;
		}

		subL->_bf = parent->_bf = 0;
	}

	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		if (bf == -1) // subLR����������
		{
			subL->_bf = 0;
			parent->_bf = 1;
			subLR->_bf = 0;
		}
		else if (bf == 1) // subLR����������
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;
		}
		else if (bf == 0) // subLR�Լ���������
		{
			parent->_bf = 0;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;
		RotateR(parent->_right);
		RotateL(parent);

		if (bf == 1)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else if (bf == 0)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	void Inorder()
	{
		_Inorder(_root);
	}

	void _Inorder(Node* root)
	{
		if (root == nullptr)
			return;

		_Inorder(root->_left);
		/*cout << root->_kv.first << ":" << root->_kv.second << endl;*/
		_Inorder(root->_right);
	}

	int Hight(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}
		int leftHight = Hight(root->_left);
		int rightHight = Hight(root->_right);

		return leftHight > rightHight ? leftHight + 1 : rightHight + 1;
	}

	bool IsBalance()
	{
		return IsBalance(_root);
	}

	bool IsBalance(Node* root)
	{
		if (root == nullptr)
		{
			return true;
		}
		int leftHight = Hight(root->_left);
		int rightHight = Hight(root->_right);
		return abs(leftHight - rightHight) < 2
			&& IsBalance(root->_left)
			&& IsBalance(root->_right);
	}

private:
	Node* _root = nullptr;
};

void TestAVLTree1()
{

	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (auto e : a)
	{
		t.Insert(make_pair(e, e));
	}

	t.Inorder();

	cout << t.IsBalance() << endl;
}

void TestAVLTree2()
{
	srand(time(0));
	const size_t N = 100000;
	AVLTree<int, int> t;
	for (size_t i = 0; i < N; ++i)
	{
		size_t x = rand();
		t.Insert(make_pair(x, x));
		//cout << t.IsBalance() << endl;
	}

	//t.Inorder();

	cout << t.IsBalance() << endl;
}


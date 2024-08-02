#pragma once

#include <utility>

// AVL���ڵ㶨��
template<typename K, typename V>
struct AVLTreeNode {
    std::pair<K, V> _kv; // �ڵ�洢�ļ�ֵ��
    AVLTreeNode<K, V>* _left; // ���ӽڵ�
    AVLTreeNode<K, V>* _right; // ���ӽڵ�
    AVLTreeNode<K, V>* _parent; // ���ڵ�
    int _bf; // ƽ�����ӣ����ڱ�������ƽ��

    // ���캯��
    AVLTreeNode(const std::pair<K, V>& kv)
        : _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0) {
        // ��ʼ���ڵ�
    }
};

// AVL������
template<typename K, typename V>
struct AVLTree {
    typedef AVLTreeNode<K, V> Node; // ʹ��typedef��AVL���ڵ������

public:
    // �����ֵ��
    bool Insert(const std::pair<K, V>& kv) {
        if (_root == nullptr) {
            _root = new Node(kv);
            return true;
        }

        Node* parent = nullptr; // ���ڼ�¼��ǰ�ڵ�ĸ��ڵ�
        Node* cur = _root; // �Ӹ��ڵ㿪ʼ����

        // ���Ҳ���λ��
        while (cur) {
            if (cur->_kv.first > kv.first) { // �����С�ڵ�ǰ�ڵ�ļ���������������
                parent = cur;
                cur = cur->_left;
            }
            else if (cur->_kv.first < kv.first) { // ��������ڵ�ǰ�ڵ�ļ���������������
                parent = cur;
                cur = cur->_right;
            }
            else { // ������Ѵ��ڣ�����ʧ��
                return false;
            }
        }

        cur = new Node(kv); // �����½ڵ�

        // ���ݼ�ֵ�Ĵ�С�����½ڵ���뵽���ڵ����������������
        if (parent->_kv.first < kv.first) {
            parent->_right = cur;
            cur->_parent = parent;
        }
        else {
            parent->_left = cur;
            cur->_parent = parent;
        }

        // ����ƽ������
        while (parent) {
            if (cur == parent->_left) {
                parent->_bf--; // ����½ڵ������ӽڵ㣬���ڵ��ƽ�����Ӽ�1
            }
            else {
                parent->_bf++; // ����½ڵ������ӽڵ㣬���ڵ��ƽ�����Ӽ�1
            }

            // �ж��Ƿ���Ҫ������ת����
            if (parent->_bf == 0) { // ���ƽ������Ϊ0�������߶�δ�䣬�������
                break;
            }
            else if (parent->_bf == 1 || parent->_bf == -1) { // ���ƽ������Ϊ1��-1����Ҫ�������ϸ���
                cur = parent;
                parent = parent->_parent;
            }
            else if (parent->_bf == -2 || parent->_bf == 2) { // ���ƽ������Ϊ-2��2����Ҫ������ת����
                Rotate(parent);
            }
            else {
                assert(false); // ��Ӧ�ﵽ����
            }
        }
        return true;
    }

    // �������
    void Inorder() {
        _Inorder(_root);
    }

    // �ݹ��������
    void _Inorder(Node* root) {
        if (root == nullptr) {
            return;
        }
        _Inorder(root->_left);
        // �����������ڵ�ļ�ֵ��
        // std::cout << root->_kv.first << ":" << root->_kv.second << std::endl;
        _Inorder(root->_right);
    }

    // �������ĸ߶�
    int Height(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        int leftHeight = Height(root->_left);
        int rightHeight = Height(root->_right);
        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }

    // ������Ƿ�ƽ��
    bool IsBalance() {
        return IsBalance(_root);
    }

    // �ݹ������Ƿ�ƽ��
    bool IsBalance(Node* root) {
        if (root == nullptr) {
            return true;
        }
        int leftHeight = Height(root->_left);
        int rightHeight = Height(root->_right);
        return abs(leftHeight - rightHeight) < 2
            && IsBalance(root->_left)
            && IsBalance(root->_right);
    }

    // ��ת����������ƽ�����ӽ�����Ӧ����������������������������
    void Rotate(Node* parent) {
        if (parent->_bf == 2) { // ����������������2����Ҫ����������������ת
            if (parent->_right->_bf < 0) { // �����������������ߣ���Ҫ������������
                RotateR(parent->_right);
                RotateL(parent);
            }
            else { // ֻ��Ҫ����
                RotateL(parent);
            }
        }
        else if (parent->_bf == -2) { // ����������������2����Ҫ����������������ת
            if (parent->_left->_bf > 0) { // �����������������ߣ���Ҫ������������
                RotateL(parent->_left);
                RotateR(parent);
            }
            else { // ֻ��Ҫ����
                RotateR(parent);
            }
        }
    }

    // ����
    void RotateL(Node* parent) {
        Node* subR = parent->_right; // ���ӽڵ�
        Node* subRL = subR->_left; // ���ӽڵ�����ӽڵ�

        // ���¸��ڵ�����ӽڵ�����ӽڵ�����ӽڵ�
        parent->_right = subRL;
        if (subRL) {
            subRL->_parent = parent;
        }

        // ������ת
        subR->_left = parent;
        parent->_parent = subR;

        // ���¸��ڵ�򸸽ڵ���ӽڵ�
        Node* ppNode = parent->_parent;
        if (_root == parent) {
            _root = subR;
            subR->_parent = nullptr;
        }
        else {
            if (ppNode->_left == parent) {
                ppNode->_left = subR;
            }
            else {
                ppNode->_right = subR;
            }
            subR->_parent = ppNode;
        }

        // ����ƽ������
        parent->_bf = subR->_bf = 0;
    }

    // ����
    void RotateR(Node* parent) {
        Node* subL = parent->_left; // ���ӽڵ�
        Node* subLR = subL->_right; // ���ӽڵ�����ӽڵ�

        // ���¸��ڵ�����ӽڵ�����ӽڵ�����ӽڵ�
        parent->_left = subLR;
        if (subLR) {
            subLR->_parent = parent;
        }

        // ������ת
        subL->_right = parent;
        parent->_parent = subL;

        // ���¸��ڵ�򸸽ڵ���ӽڵ�
        Node* ppNode = parent->_parent;
        if (ppNode == nullptr) { // ������ڵ�Ϊ�գ�˵����ǰ�ڵ�Ϊ���ڵ�
            _root = subL;
            _root->_parent = nullptr;
        }
        else {
            if (ppNode->_left == parent) {
                ppNode->_left = subL;
            }
            else {
                ppNode->_right = subL;
            }
            subL->_parent = ppNode;
        }

        // ����ƽ������
        subL->_bf = parent->_bf = 0;
    }

private:
    Node* _root = nullptr; // ���ڵ�
};

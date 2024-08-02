#pragma once

#include <utility>

// AVL树节点定义
template<typename K, typename V>
struct AVLTreeNode {
    std::pair<K, V> _kv; // 节点存储的键值对
    AVLTreeNode<K, V>* _left; // 左子节点
    AVLTreeNode<K, V>* _right; // 右子节点
    AVLTreeNode<K, V>* _parent; // 父节点
    int _bf; // 平衡因子，用于保持树的平衡

    // 构造函数
    AVLTreeNode(const std::pair<K, V>& kv)
        : _kv(kv), _left(nullptr), _right(nullptr), _parent(nullptr), _bf(0) {
        // 初始化节点
    }
};

// AVL树定义
template<typename K, typename V>
struct AVLTree {
    typedef AVLTreeNode<K, V> Node; // 使用typedef简化AVL树节点的类型

public:
    // 插入键值对
    bool Insert(const std::pair<K, V>& kv) {
        if (_root == nullptr) {
            _root = new Node(kv);
            return true;
        }

        Node* parent = nullptr; // 用于记录当前节点的父节点
        Node* cur = _root; // 从根节点开始查找

        // 查找插入位置
        while (cur) {
            if (cur->_kv.first > kv.first) { // 如果键小于当前节点的键，向左子树查找
                parent = cur;
                cur = cur->_left;
            }
            else if (cur->_kv.first < kv.first) { // 如果键大于当前节点的键，向右子树查找
                parent = cur;
                cur = cur->_right;
            }
            else { // 如果键已存在，插入失败
                return false;
            }
        }

        cur = new Node(kv); // 创建新节点

        // 根据键值的大小，将新节点插入到父节点的左子树或右子树
        if (parent->_kv.first < kv.first) {
            parent->_right = cur;
            cur->_parent = parent;
        }
        else {
            parent->_left = cur;
            cur->_parent = parent;
        }

        // 更新平衡因子
        while (parent) {
            if (cur == parent->_left) {
                parent->_bf--; // 如果新节点是左子节点，父节点的平衡因子减1
            }
            else {
                parent->_bf++; // 如果新节点是右子节点，父节点的平衡因子加1
            }

            // 判断是否需要进行旋转调整
            if (parent->_bf == 0) { // 如果平衡因子为0，子树高度未变，无需调整
                break;
            }
            else if (parent->_bf == 1 || parent->_bf == -1) { // 如果平衡因子为1或-1，需要继续向上更新
                cur = parent;
                parent = parent->_parent;
            }
            else if (parent->_bf == -2 || parent->_bf == 2) { // 如果平衡因子为-2或2，需要进行旋转调整
                Rotate(parent);
            }
            else {
                assert(false); // 不应达到此行
            }
        }
        return true;
    }

    // 中序遍历
    void Inorder() {
        _Inorder(_root);
    }

    // 递归中序遍历
    void _Inorder(Node* root) {
        if (root == nullptr) {
            return;
        }
        _Inorder(root->_left);
        // 这里可以输出节点的键值对
        // std::cout << root->_kv.first << ":" << root->_kv.second << std::endl;
        _Inorder(root->_right);
    }

    // 计算树的高度
    int Height(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        int leftHeight = Height(root->_left);
        int rightHeight = Height(root->_right);
        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }

    // 检查树是否平衡
    bool IsBalance() {
        return IsBalance(_root);
    }

    // 递归检查树是否平衡
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

    // 旋转方法，根据平衡因子进行相应的左旋、右旋或左右旋、右左旋
    void Rotate(Node* parent) {
        if (parent->_bf == 2) { // 右子树比左子树高2，需要进行左旋或左右旋转
            if (parent->_right->_bf < 0) { // 右子树的左子树更高，需要先右旋再左旋
                RotateR(parent->_right);
                RotateL(parent);
            }
            else { // 只需要左旋
                RotateL(parent);
            }
        }
        else if (parent->_bf == -2) { // 左子树比右子树高2，需要进行右旋或右左旋转
            if (parent->_left->_bf > 0) { // 左子树的右子树更高，需要先左旋再右旋
                RotateL(parent->_left);
                RotateR(parent);
            }
            else { // 只需要右旋
                RotateR(parent);
            }
        }
    }

    // 左旋
    void RotateL(Node* parent) {
        Node* subR = parent->_right; // 右子节点
        Node* subRL = subR->_left; // 右子节点的左子节点

        // 更新父节点的右子节点和右子节点的左子节点
        parent->_right = subRL;
        if (subRL) {
            subRL->_parent = parent;
        }

        // 进行旋转
        subR->_left = parent;
        parent->_parent = subR;

        // 更新根节点或父节点的子节点
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

        // 更新平衡因子
        parent->_bf = subR->_bf = 0;
    }

    // 右旋
    void RotateR(Node* parent) {
        Node* subL = parent->_left; // 左子节点
        Node* subLR = subL->_right; // 左子节点的右子节点

        // 更新父节点的左子节点和左子节点的右子节点
        parent->_left = subLR;
        if (subLR) {
            subLR->_parent = parent;
        }

        // 进行旋转
        subL->_right = parent;
        parent->_parent = subL;

        // 更新根节点或父节点的子节点
        Node* ppNode = parent->_parent;
        if (ppNode == nullptr) { // 如果父节点为空，说明当前节点为根节点
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

        // 更新平衡因子
        subL->_bf = parent->_bf = 0;
    }

private:
    Node* _root = nullptr; // 根节点
};

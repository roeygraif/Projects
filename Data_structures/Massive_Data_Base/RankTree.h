
#include "wet2util.h"

#ifndef WET1_RANKTREE_H
#define WET1_RANKTREE_H

template <class T>
class RankNode
{
public:
    T data_m;
    int height_m;
    RankNode<T>* left_m = nullptr;
    RankNode<T>* right_m = nullptr;
    RankNode<T>* parent_m = nullptr;
    int leftSubTreeSize_m;  // Node leftSubTreeSize - Number of nodes to the left of this node
    int rightSubTreeSize_m;  // Node rightSubTreeSize - Number of nodes to the right of this node

public:
    RankNode(T data, RankNode<T>* parent = nullptr, RankNode<T>* left = nullptr, RankNode<T>* right = nullptr, int leftSubTreeSize_m = 0, int rightSubTreeSize_m = 0);
    ~RankNode();
    T getData() const { return data_m; }
    int getHeight() const { return height_m; }
    RankNode<T>* getLeft() const { return left_m; }
    RankNode<T>* getRight() const { return right_m; }
    RankNode<T>* getParent() const { return parent_m; }
    int getLeftSubTreeSize() const { return leftSubTreeSize_m; }
    int getRightSubTreeSize() const { return rightSubTreeSize_m; }
    int getTotalSubTreeSize() const { return leftSubTreeSize_m + rightSubTreeSize_m; }
    StatusType setLeft(RankNode<T>* left)
    {
        this->left_m = left;
        return StatusType::SUCCESS;
    }
    StatusType setRight(RankNode<T>* right)
    {
        this->right_m = right;
        return StatusType::SUCCESS;
    }
    StatusType setHeight(int height)
    {
        this->height_m = height;
        return StatusType::SUCCESS;
    }
    StatusType setData(T data)
    {
        this->data_m = data;
        return StatusType::SUCCESS;
    }
    StatusType setLeftSubTreeSize(int leftSubTreeSize)
    {
        this->leftSubTreeSize_m = leftSubTreeSize;
        return StatusType::SUCCESS;
    }
    StatusType setRightSubTreeSize(int rightSubTreeSize)
    {
        this->rightSubTreeSize_m = rightSubTreeSize;
        return StatusType::SUCCESS;
    }
    StatusType setRefData(T& data)
    {
        this->data_m = data;
        return StatusType::SUCCESS;
    }
    StatusType setParent(RankNode<T>* parent)
    {
        this->parent_m = parent;
        return StatusType::SUCCESS;
    }
};

template <class T>
RankNode<T>::RankNode(T data, RankNode<T>* parent, RankNode<T>* left, RankNode<T>* right, int leftSubTreeSize, int rightSubTreeSize)
{
    this->data_m = data;
    this->parent_m = parent;
    this->left_m = left;
    this->right_m = right;
    this->height_m = 0;
    this->leftSubTreeSize_m = leftSubTreeSize;
    this->rightSubTreeSize_m = rightSubTreeSize;
}

template <class T>
RankNode<T>::~RankNode()
{
    this->left_m = nullptr;
    this->right_m = nullptr;
    this->parent_m = nullptr;
}

// we will use a tree class to represent the tree.
// the tree class will contain a pointer to the root of the tree.

template <class T>
class RankTree
{
private:
    RankNode<T>* root_m;

public:
    RankTree() : root_m(nullptr) {}
    ~RankTree();

    RankNode<T>* getRoot() const { return root_m; }
    StatusType setRoot(RankNode<T>* root)
    {
        this->root_m = root;
        return StatusType::SUCCESS;
    }
    output_t<RankNode<T>*> insert(T data);
    StatusType remove(T data);
    StatusType removeWithoutSubTreeSizeUpdate(T data);
    int rank(T data);
    output_t<RankNode<T>*> select(int index);
    output_t<RankNode<T>*> find(T data);
    StatusType updateSubTreeSizes(T data);
    RankNode<T>* findMin(RankNode<T>* node);
    RankNode<T>* findMax(RankNode<T>* node);
    RankNode<T>* findParent(RankNode<T>* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        return node->getParent();
    }
    RankNode<T>* findSuccessor(RankNode<T>* node);   // complexity O(logn)
    RankNode<T>* findPredecessor(RankNode<T>* node); // complexity O(logn)
    StatusType RRrotation(RankNode<T>* node);
    StatusType LLrotation(RankNode<T>* node);
    StatusType RLrotation(RankNode<T>* node);
    StatusType LRrotation(RankNode<T>* node);
    StatusType rebalance(RankNode<T>* node);
    StatusType updateHeight(RankNode<T>* node);
    StatusType switchNodesLocation(RankNode<T>* node1, RankNode<T>* node2);
    int getBalanceFactor(RankNode<T>* node);
    int getBalance(RankNode<T>* node);
    StatusType inOrder(RankNode<T>* node, void (*func)(T, int*, void*), int* counter, void* array);
    StatusType inOrderArray(void (*func)(T, int*, void*), void* array);
    StatusType printInOrder(RankNode<T>* node);
    void DestroyRecursive(RankNode<T>* node);
};

template <class T>
StatusType RankTree<T>::inOrderArray(void (*func)(T, int*, void*), void* array)
{
    int counter = 0;
    inOrder(root_m, func, &counter, array);
    return StatusType::SUCCESS;
}

template <class T>
StatusType RankTree<T>::inOrder(RankNode<T>* node, void (*func)(T, int*, void*), int* counter, void* array)
{
    if (node == nullptr)
    {
        return StatusType::SUCCESS;
    }
    inOrder(node->getLeft(), func, counter, array);
    func(node->getData(), counter, array);
    *counter = *counter + 1;
    inOrder(node->getRight(), func, counter, array);
    return StatusType::SUCCESS;
}

template <class T>
StatusType RankTree<T>::printInOrder(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return StatusType::SUCCESS;
    }
    printInOrder(node->getLeft());
    printf("Node data.ability = %d, data.team_id = %d, leftSubTreeSize = %d, rightSubTreeSize = %d\n", node->getData().getTeamAbility(), node->getData().getTeamId(), node->getLeftSubTreeSize(), node->getRightSubTreeSize());
    printInOrder(node->getRight());
    return StatusType::SUCCESS;
}


template <class T>
StatusType RankTree<T>::switchNodesLocation(RankNode<T>* node1, RankNode<T>* node2)
{
    if (node1 == nullptr || node2 == nullptr)
    {
        return StatusType::INVALID_INPUT;
    }
    RankNode<T>* node1Parent = node1->getParent();
    RankNode<T>* node1Left = node1->getLeft();
    RankNode<T>* node1Right = node1->getRight();
    RankNode<T>* node2Parent = node2->getParent();
    RankNode<T>* node2Left = node2->getLeft();
    RankNode<T>* node2Right = node2->getRight();
    // lets assume 4 cases:
    // 1. node1 and node2 are seperate nodes, they are not connected to each other.
    // 2. node1 is the parent of node2.
    // 3. node2 is the parent of node1.
    // 4. node1 and node2 are siblings.
    // case 1:
    if (node1Parent != node2 && node1Left != node2 && node1Right != node2 && node1Parent != node2Parent)
    {
        if (node1Parent == nullptr)
        {
            this->setRoot(node2);
        }
        else
        {
            if (node1Parent->getLeft() == node1)
            {
                node1Parent->setLeft(node2);
            }
            else
            {
                node1Parent->setRight(node2);
            }
        }
        if (node2Parent == nullptr)
        {
            this->setRoot(node1);
        }
        else
        {
            if (node2Parent->getLeft() == node2)
            {
                node2Parent->setLeft(node1);
            }
            else
            {
                node2Parent->setRight(node1);
            }
        }
        node1->setParent(node2Parent);
        node1->setLeft(node2Left);
        node1->setRight(node2Right);
        node2->setParent(node1Parent);
        node2->setLeft(node1Left);
        node2->setRight(node1Right);
        if (node1Left != nullptr)
        {
            node1Left->setParent(node2);
        }
        if (node1Right != nullptr)
        {
            node1Right->setParent(node2);
        }
        if (node2Left != nullptr)
        {
            node2Left->setParent(node1);
        }
        if (node2Right != nullptr)
        {
            node2Right->setParent(node1);
        }
    }
    // case 2:
    if (node1 == node2Parent)
    {
        if (node1Parent == nullptr)
        {
            this->setRoot(node2);
            node2->setParent(nullptr);
            node1->setParent(node2);
        }
        else
        {
            if (node1Parent->getLeft() == node1)
            {
                node1Parent->setLeft(node2);
                node2->setParent(node1Parent);
            }
            else
            {
                node1Parent->setRight(node2);
                node2->setParent(node1Parent);
            }
            node1->setParent(node2);
        }
        node1->setLeft(node2Left);
        node1->setRight(node2Right);
        if (node2 == node1Left)
        {
            node2->setLeft(node1);
            node2->setRight(node1Right);
            if (node1Right != nullptr)
            {
                node1Right->setParent(node2);
            }
        }
        else
        {
            node2->setLeft(node1Left);
            node2->setRight(node1);
            if (node1Left != nullptr)
            {
                node1Left->setParent(node2);
            }
        }
    }
    // case 3:
    if (node2 == node1Parent)
    {
        switchNodesLocation(node2, node1);
    }
    // case 4:
    if (node1Parent == node2Parent)
    {
        if (node1Parent == nullptr && node2Parent == nullptr)
        {
            return StatusType::FAILURE;
        }
        node1->setLeft(node2Left);
        node1->setRight(node2Right);
        node2->setLeft(node1Left);
        node2->setRight(node1Right);
    }
    T temp;

    temp = node1->data_m;
    node1->data_m = node2->data_m;
    node2->data_m = temp;

    int tempHeight = node1->getHeight();
    node1->setHeight(node2->getHeight());
    node2->setHeight(tempHeight);

    int tempLeftSubTreeSize = node1->getLeftSubTreeSize();
    int tempRightSubTreeSize = node1->getRightSubTreeSize();
    node1->setLeftSubTreeSize(node2->getLeftSubTreeSize());
    node2->setLeftSubTreeSize(tempLeftSubTreeSize);
    node1->setRightSubTreeSize(node2->getRightSubTreeSize());
    node2->setRightSubTreeSize(tempRightSubTreeSize);

    return StatusType::SUCCESS;
}
// the function ~Tree() will free all the memory allocated for the tree.
// the complexity of the ~Tree() must be O(n) where n is the number of nodes in the tree.
template <class T>
void RankTree<T>::DestroyRecursive(RankNode<T>* node)
{
    if (node)
    {
        DestroyRecursive(node->getLeft());
        DestroyRecursive(node->getRight());
        delete node;
    }
}
template <class T>
RankTree<T>::~RankTree()
{
    DestroyRecursive(root_m);
}

// the time complexity of the insert function is O(log(n)).
template <class T>
output_t<RankNode<T>*> RankTree<T>::insert(T data)
{
    RankNode<T>* newNode = new RankNode<T>(data);
    if (newNode == nullptr)
    {
        return output_t<RankNode<T>*>(StatusType::ALLOCATION_ERROR);
    }
    if (root_m == nullptr)
    {
        root_m = newNode;
        return output_t<RankNode<T>*>(newNode);
    }
    RankNode<T>* current = root_m;
    RankNode<T>* parent = nullptr;
    while (current != nullptr)
    {
        parent = current;
        if (data < current->getData())
        {
            // When turning left (while searching for data), we follow the rule l(v) <- l(v)+1
            current->setLeftSubTreeSize(current->getLeftSubTreeSize() + 1);
            current = current->getLeft();
        }
        else if (data > current->getData())
        {
            // When turning right (while searching for data), we follow the rule r(v) <- r(v)+1
            current->setRightSubTreeSize(current->getRightSubTreeSize() + 1);
            current = current->getRight();
        }
        else
        {
            delete newNode;
            return output_t<RankNode<T>*>(StatusType::FAILURE);
        }
    }
    newNode->setParent(parent);
    if (data < parent->getData())
    {
        parent->setLeft(newNode);
    }
    else
    {
        parent->setRight(newNode);
    }
    StatusType status = rebalance(newNode);
    if (status != StatusType::SUCCESS)
    {
        return output_t<RankNode<T>*>(status);
    }
    return output_t<RankNode<T>*>(newNode);
}


template <class T>
StatusType RankTree<T>::remove(T data)
{
    output_t<RankNode<T>*> output = find(data);

    if (output.status() != StatusType::SUCCESS)
    {
        return output.status();
    }
    RankNode<T>* node = output.ans();
    if (node == nullptr)
    {
        return StatusType::FAILURE;
    }
    RankNode<T>* parent = node->getParent();
    if (node->getLeft() == nullptr && node->getRight() == nullptr)
    {
        
        if (parent == nullptr)
        {
            setRoot(nullptr);
        }
        else if (parent->getLeft() == node)
        {
            parent->setLeft(nullptr);
        }
        else
        {
            parent->setRight(nullptr);
        }
        T temp = T();
        temp = node->data_m;
        delete node;
        // In this case (node has no children, i.e. is leaf), after removing node we search for successor data in tree, updating node leftSubTreeSizes when we go left (as seen in tutorial)
        updateSubTreeSizes(data);
        return rebalance(parent);
    }
    else if (node->getLeft() == nullptr || node->getRight() == nullptr)
    {

        RankNode<T>* child = node->getLeft() == nullptr ? node->getRight() : node->getLeft();
        if (parent == nullptr)
        {
            setRoot(child);
        }
        else if (parent->getLeft() == node)
        {
            parent->setLeft(child);
        }
        else
        {
            parent->setRight(child);
        }
        child->setParent(parent);
        T temp;
        temp = node->data_m;

        delete node;
        // In this case (node has only child), after removing node we search for successor data in tree, updating node leftSubTreeSizes when we go left (as seen in tutorial)
        updateSubTreeSizes(data);
        return rebalance(parent);
    }
    else
    {
        RankNode<T>* successor = findSuccessor(node);
        // In this case (node has two children), after finding successor we search for successor data in tree, updating node leftSubTreeSizes when we go left (as seen in tutorial)
        updateSubTreeSizes(successor->getData());
        T temp = T();
        temp = successor->data_m;
        switchNodesLocation(node, successor);
        removeWithoutSubTreeSizeUpdate(temp);
        T temp2 = T();
        temp2 = temp;
        successor->data_m = temp2;
        return StatusType::SUCCESS;
    }
}

// Removes node with data, without changing nodes' leftSubTreeSizes
template <class T>
StatusType RankTree<T>::removeWithoutSubTreeSizeUpdate(T data)
{
    output_t<RankNode<T>*> output = find(data);

    if (output.status() != StatusType::SUCCESS)
    {
        return output.status();
    }
    RankNode<T>* node = output.ans();
    if (node == nullptr)
    {
        return StatusType::FAILURE;
    }
    RankNode<T>* parent = node->getParent();
    if (node->getLeft() == nullptr && node->getRight() == nullptr)
    {

        if (parent == nullptr)
        {
            setRoot(nullptr);
        }
        else if (parent->getLeft() == node)
        {
            parent->setLeft(nullptr);
        }
        else
        {
            parent->setRight(nullptr);
        }
        T temp = T();
        temp = node->data_m;
        delete node;
        return rebalance(parent);
    }
    else if (node->getLeft() == nullptr || node->getRight() == nullptr)
    {

        RankNode<T>* child = node->getLeft() == nullptr ? node->getRight() : node->getLeft();
        if (parent == nullptr)
        {
            setRoot(child);
        }
        else if (parent->getLeft() == node)
        {
            parent->setLeft(child);
        }
        else
        {
            parent->setRight(child);
        }
        child->setParent(parent);
        T temp;
        temp = node->data_m;

        delete node;
        return rebalance(parent);
    }
    else
    {
        RankNode<T>* successor = findSuccessor(node);
        T temp = T();
        temp = successor->data_m;
        switchNodesLocation(node, successor);
        remove(temp);
        T temp2 = T();
        temp2 = temp;
        successor->data_m = temp2;
        return StatusType::SUCCESS;
    }
}

// the time complexity of the find function is O(log(n)).
template <class T>
output_t<RankNode<T>*> RankTree<T>::find(T data)
{
    RankNode<T>* current = root_m;
    while (current != nullptr)
    {
        if (current->data_m == data)
        {
            return output_t<RankNode<T>*>(current);
        }
        if (current->data_m > data)
        {
            current = current->getLeft();
        }
        else if (current->data_m < data)
        {
            current = current->getRight();
        }
        else
        {
            return output_t<RankNode<T>*>(current);
        }
    }
    return output_t<RankNode<T>*>(StatusType::FAILURE);
}

// the time complexity of the rank function is O(log(n)).
// Method returns rank of given data, following the algorithm seen at class
template <class T>
int RankTree<T>::rank(T data)
{
    int current_rank = 0;
    RankNode<T>* current = root_m;

    while (current != nullptr)
    {
        if (current->data_m == data)
        {
            current_rank += current->getLeftSubTreeSize() + 1;
            return current_rank;
        }
        if (current->data_m > data)
        {
            current = current->getLeft();
        }
        else if (current->data_m < data)
        {
            current_rank += current->getLeftSubTreeSize() + 1;
            current = current->getRight();
            
        }
        else
        {
            return current_rank;
        }
    }
    return -1; // Failure
}

// the time complexity of the select function is O(log(n)).
// Method returns data (node) at given index, following the algorithm seen at class
template <class T>
output_t<RankNode<T>*> RankTree<T>::select(int index)
{
    if (index <= 0)
    {
        return nullptr;
    }

    int k = index;
    RankNode<T>* current = root_m;

    while (current != nullptr)
    {
        if (current->getLeftSubTreeSize() == k - 1)
        {
            return output_t<RankNode<T>*>(current);
        }
        if (current->getLeftSubTreeSize() > k - 1)
        {
            current = current->getLeft();
        }
        else if (current->getLeftSubTreeSize() < k - 1)
        {
            k = k - current->getLeftSubTreeSize() - 1;
            current = current->getRight();
        }
        else
        {
            return output_t<RankNode<T>*>(StatusType::FAILURE);
        }
    }
    return output_t<RankNode<T>*>(StatusType::FAILURE);
}

// the time complexity of the update leftSubTreeSize function is O(log(n)).
// This method updates leftSubTreeSizes upon removal of node, preforming w(v) <- w(v)-1 when we turn left from vertive v (while searching for data), as seen in tutorial
template <class T>
StatusType RankTree<T>::updateSubTreeSizes(T data)
{
    RankNode<T>* current = root_m;
    while (current != nullptr)
    {
        if (current->data_m == data)
        {
            return StatusType::SUCCESS;
        }
        if (current->data_m > data)
        {
            if (current->getLeftSubTreeSize() > 0) {
                current->setLeftSubTreeSize(current->getLeftSubTreeSize() - 1);
            }
            current = current->getLeft();
        }
        else if (current->data_m < data)
        {
            if (current->getRightSubTreeSize() > 0) {
                current->setRightSubTreeSize(current->getRightSubTreeSize() - 1);
            }
            current = current->getRight();
        }
        else
        {
            return StatusType::SUCCESS;
        }
    }
    return StatusType::FAILURE;
}

// the time complexity of the find function is O(log(n)).
template <class T>
RankNode<T>* RankTree<T>::findMin(RankNode<T>* node)
{
    RankNode<T>* curr = node;
    while (curr->getLeft() != nullptr)
    {
        curr = curr->getLeft();
    }
    return curr;
}

// the time complexity of the findMin function is O(log(n)).
template <class T>
RankNode<T>* RankTree<T>::findMax(RankNode<T>* node)
{
    RankNode<T>* curr = node;
    if (curr == nullptr)
    {
        return nullptr;
    }
    while (curr->getRight() != nullptr)
    {
        curr = curr->getRight();
    }
    return curr;
}
// the time complexity of the findMax function is O(log(n)).

template <class T>
StatusType RankTree<T>::RRrotation(RankNode<T>* node)
{
    RankNode<T>* parent = node->getParent();
    RankNode<T>* right = node->getRight();
    RankNode<T>* rightLeft = right->getLeft();
    
    if (parent == nullptr)
    {
        setRoot(right);
    }
    else if (parent->getLeft() == node)
    {
        parent->setLeft(right);
    }
    else
    {
        parent->setRight(right);
    }
    right->setParent(parent);
    right->setLeft(node);
    node->setParent(right);
    node->setRight(rightLeft);
    if (rightLeft != nullptr)
    {
        rightLeft->setParent(node);
        // Update sub-tree size in node (parent and rightLeft sub-tree sizes stay unchanged)
        node->setRightSubTreeSize(rightLeft->getTotalSubTreeSize() + 1);
    }
    else {
        node->setRightSubTreeSize(0);
    }
    // Update sub-tree size in right (parent and rightLeft sub-tree sizes stay unchanged)
    right->setLeftSubTreeSize(node->getTotalSubTreeSize() + 1);
    // Update node/right heights
    updateHeight(node);
    updateHeight(right);
    return StatusType::SUCCESS;
}
// the time complexity of the RRrotation function is O(1).

template <class T>
StatusType RankTree<T>::LLrotation(RankNode<T>* node)
{
    RankNode<T>* parent = node->getParent();
    RankNode<T>* left = node->getLeft();
    RankNode<T>* leftRight = left->getRight();
    
    if (parent == nullptr)
    {
        setRoot(left);
    }
    else if (parent->getLeft() == node)
    {
        parent->setLeft(left);
    }
    else
    {
        parent->setRight(left);
    }
    left->setParent(parent);
    left->setRight(node);
    node->setParent(left);
    node->setLeft(leftRight);
    
    if (leftRight != nullptr)
    {
        leftRight->setParent(node);
        // Update sub-tree size in node (parent and leftRight sub-tree sizes stay unchanged)
        node->setLeftSubTreeSize(leftRight->getTotalSubTreeSize() + 1);
    }
    else {
        node->setLeftSubTreeSize(0);
    }
    // Update sub-tree size in left (parent and leftRight sub-tree sizes stay unchanged)
    left->setRightSubTreeSize(node->getTotalSubTreeSize() + 1);
    // Update node/left heights
    updateHeight(node);
    updateHeight(left);
    return StatusType::SUCCESS;
}
// the time complexity of the LLrotation function is O(1).

template <class T>
StatusType RankTree<T>::LRrotation(RankNode<T>* node)
{
    RankNode<T>* left = node->getLeft();
    RRrotation(left);
    LLrotation(node);
    return StatusType::SUCCESS;
}
// the time complexity of the LRrotation function is O(1).

template <class T>
StatusType RankTree<T>::RLrotation(RankNode<T>* node)
{
    RankNode<T>* right = node->getRight();
    LLrotation(right);
    RRrotation(node);
    return StatusType::SUCCESS;
}
// the time complexity of the RLrotation function is O(1).

template <class T>
StatusType RankTree<T>::rebalance(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return StatusType::SUCCESS;
    }
    int balance = getBalance(node);
    if (balance > 1)
    {
        if (getBalance(node->getLeft()) >= 0)
        {
            LLrotation(node);
        }
        else
        {
            LRrotation(node);
        }
    }
    else if (balance < -1)
    {
        if (getBalance(node->getRight()) <= 0)
        {
            RRrotation(node);
        }
        else
        {
            RLrotation(node);
        }
    }
    else
    {
        updateHeight(node);
    }
    return rebalance(node->getParent());
}

template <class T>
int RankTree<T>::getBalance(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    if (node->getLeft() == nullptr && node->getRight() == nullptr)
    {
        return 0;
    }
    else if (node->getLeft() == nullptr)
    {
        return -(node->getRight()->getHeight());
    }
    else if (node->getRight() == nullptr)
    {
        return node->getLeft()->getHeight();
    }
    else
    {
        return node->getLeft()->getHeight() - node->getRight()->getHeight();
    }
}

template <class T>
int RankTree<T>::getBalanceFactor(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return 0;
    }
    int leftHeight = node->getLeft() == nullptr ? 0 : node->getLeft()->getHeight();
    int rightHeight = node->getRight() == nullptr ? 0 : node->getRight()->getHeight();
    return leftHeight - rightHeight;
}
// the time complexity of the getBalanceFactor function is O(1).
template <class T>
StatusType RankTree<T>::updateHeight(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return StatusType::SUCCESS;
    }
    int leftHeight = node->getLeft() == nullptr ? 0 : node->getLeft()->getHeight();
    int rightHeight = node->getRight() == nullptr ? 0 : node->getRight()->getHeight();
    int temp = leftHeight > rightHeight ? leftHeight : rightHeight;
    node->setHeight(temp + 1);
    return StatusType::SUCCESS;
}
// the time complexity of the updateHeight function is O(1).

template <class T>
RankNode<T>* findParent(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    return node->getParent();
}

template <class T>
RankNode<T>* RankTree<T>::findSuccessor(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->getRight() != nullptr)
    {
        return findMin(node->getRight());
    }
    RankNode<T>* parent = node->getParent();
    while (parent != nullptr && node == parent->getRight())
    {
        node = parent;
        parent = parent->getParent();
    }
    return parent;
}

template <class T>
RankNode<T>* RankTree<T>::findPredecessor(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (node->left_m != nullptr)
    {

        return findMax(node->getLeft());
    }

    RankNode<T>* parent = node->getParent();

    while (parent != nullptr && node == parent->getLeft())
    {
        node = parent;

        parent = node->getParent();
    }

    return parent;
}

template <class T>
RankNode<T>* findNext(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->getRight() != nullptr)
    {
        return findMin(node->getRight());
    }
    RankNode<T>* parent = node->getParent();
    while (parent != nullptr && node == parent->getRight())
    {
        node = parent;
        parent = parent->getParent();
    }
    return parent;
}

// the time complexity of the findNext function is O(log(n)).

template <class T>
RankNode<T>* findPrev(RankNode<T>* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->getLeft() != nullptr)
    {
        return findMax(node->getLeft());
    }
    RankNode<T>* parent = node->getParent();
    while (parent != nullptr && node == parent->getLeft())
    {
        node = parent;
        parent = parent->getParent();
    }
    return parent;
}

#endif // WET1_RANKTREE_H

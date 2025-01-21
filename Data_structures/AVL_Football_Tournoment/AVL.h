
#include "wet1util.h"


#ifndef WET1_AVL_H
#define WET1_AVL_H



template<class T>
class Node{
    public:
    T data_m;
    int height_m;
    Node<T>* left_m=nullptr;
    Node<T>* right_m=nullptr;
    Node<T>* parent_m=nullptr;

    public:
    Node(T data, Node<T>* parent = nullptr, Node<T>* left = nullptr, Node<T>* right = nullptr);
    ~Node();
    T getData() const {return data_m;}
    int getHeight() const {return height_m;}
    Node<T>* getLeft() const {return left_m;}
    Node<T>* getRight() const {return right_m;}
    StatusType setLeft(Node<T>* left) {this->left_m = left; return StatusType::SUCCESS;}
    StatusType setRight(Node<T>* right) {this->right_m = right; return StatusType::SUCCESS;}
    StatusType setHeight(int height) {this->height_m = height; return StatusType::SUCCESS;}
    StatusType setData(T data) {this->data_m = data; return StatusType::SUCCESS;}
    StatusType setRefData (T & data) {this->data_m = data; return StatusType::SUCCESS;}
    Node<T>* getParent() const {return parent_m;}
    StatusType setParent(Node<T>* parent) {this->parent_m = parent; return StatusType::SUCCESS;}
    

};



template<class T>
Node<T>::Node(T data, Node<T>* parent, Node<T>* left, Node<T>* right){
    this->data_m = data;
    this->parent_m = parent;
    this->left_m = left;
    this->right_m = right;
    this->height_m = 0;
}

template<class T>
Node<T>::~Node(){
    this->left_m = nullptr;
    this->right_m = nullptr;
    this->parent_m = nullptr;

}


// we will use a tree class to represent the tree.
// the tree class will contain a pointer to the root of the tree.

template<class T>
class Tree{
    private:
    Node<T>* root_m;

    public:
    Tree():root_m(nullptr){}
    ~Tree();

    Node<T>* getRoot() const {return root_m;}
    StatusType setRoot(Node<T>* root) {this->root_m = root; return StatusType::SUCCESS;}
    output_t<Node<T>*> insert(T data);
    StatusType remove(T data);
    output_t<Node<T>*> find(T data);
    Node<T>* findMin(Node<T>* node);
    Node<T>* findMax(Node<T>* node);
    Node<T>* findParent(Node<T>* node){
        if(node == nullptr){
            return nullptr;
        }
        return node->getParent();
    }
    Node<T>* findSuccessor(Node<T>* node); // complexity O(logn)
    Node<T>* findPredecessor(Node<T>* node); // complexity O(logn)
    StatusType RRrotation(Node<T>* node);
    StatusType LLrotation(Node<T>* node);
    StatusType RLrotation(Node<T>* node);
    StatusType LRrotation(Node<T>* node);
    StatusType rebalance(Node<T>* node);
    StatusType updateHeight(Node<T>* node);
    StatusType switchNodesLocation(Node<T>* node1, Node<T>* node2);
    int getBalanceFactor(Node<T>* node);
    int getBalance(Node<T>* node);
    StatusType inOrder(Node<T>* node, void (*func)(T,int*,void*), int* counter, void* array);
    StatusType inOrderArray(void (*func)(T,int*,void*), void* array);
    void DestroyRecursive(Node<T>* node);
    // we want to create a method that will create a new tree from a sorted array of pointers to nodes.
    // we will use the inOrderArray method to create the array.
    // the creation of the tree from sorted array will be done in O(n) time.
    // while the tree is created, we will update the height of each node, its parent and its children.
    Node<T>* createTreeFromSortedArray(Node<T>** array, int start, int end,Node<T>* parent = nullptr);
    
};

template<class T>
Node<T>* Tree<T>::createTreeFromSortedArray(Node<T>** array, int start, int end, Node<T>* parent){
    if(start > end || start < 0 || end < 0){
        return nullptr;
    }
    int mid = (start + end)/2;
    Node<T>* node = array[mid];
    if (parent == nullptr){
        this->setRoot(node);
    }
    else{
        node->setParent(parent);
    }
    node->setLeft(createTreeFromSortedArray(array, start, mid-1, node));
    node->setRight(createTreeFromSortedArray(array, mid+1, end, node));
    updateHeight(node);
    return node;
}


template<class T>
StatusType Tree<T>::inOrderArray(void (*func)(T,int*,void*), void* array){
    int counter = 0;
    inOrder(root_m, func, &counter, array);
    return StatusType::SUCCESS;
}



template<class T>
StatusType Tree<T>::inOrder(Node<T>* node, void (*func)(T,int*,void*), int* counter , void* array){
    if(node == nullptr){
        return StatusType::SUCCESS;
    }
    inOrder(node->getLeft(), func, counter, array);
    func(node->getData(), counter, array);
    *counter = *counter + 1;
    inOrder(node->getRight(), func, counter, array);
    return StatusType::SUCCESS;
}


template<class T>
StatusType Tree<T>::switchNodesLocation(Node<T>* node1, Node<T>* node2){
    if(node1 == nullptr || node2 == nullptr){
        return StatusType::INVALID_INPUT;
    }
    Node<T>* node1Parent= node1->getParent();
    Node<T>* node1Left = node1->getLeft();
    Node<T>* node1Right = node1->getRight();
    Node<T>* node2Parent= node2->getParent();
    Node<T>* node2Left = node2->getLeft();
    Node<T>* node2Right = node2->getRight();
    // lets assume 4 cases:
    // 1. node1 and node2 are seperate nodes, they are not connected to each other.
    // 2. node1 is the parent of node2.
    // 3. node2 is the parent of node1.
    // 4. node1 and node2 are siblings.
    // case 1:
    if (node1Parent != node2 && node1Left != node2 && node1Right != node2 && node1Parent != node2Parent){
        if (node1Parent == nullptr){
            this->setRoot(node2);
        }
        else{
            if (node1Parent->getLeft() == node1){
                node1Parent->setLeft(node2);
            }
            else{
                node1Parent->setRight(node2);
            }
        }
        if (node2Parent == nullptr){
            this->setRoot(node1);
        }
        else{
            if (node2Parent->getLeft() == node2){
                node2Parent->setLeft(node1);
            }
            else{
                node2Parent->setRight(node1);
            }
        }
        node1->setParent(node2Parent);
        node1->setLeft(node2Left); 
        node1->setRight(node2Right);
        node2->setParent(node1Parent);
        node2->setLeft(node1Left);
        node2->setRight(node1Right);
        if (node1Left != nullptr){
            node1Left->setParent(node2);
        }
        if (node1Right != nullptr){
            node1Right->setParent(node2);
        }
        if (node2Left != nullptr){
            node2Left->setParent(node1);
        }
        if (node2Right != nullptr){
            node2Right->setParent(node1);
        }

    }
    //case 2:
    if (node1 == node2Parent){
        if (node1Parent == nullptr){
            this->setRoot(node2);
            node2->setParent(nullptr);
            node1->setParent(node2);
        }
        else{
            if (node1Parent->getLeft() == node1){
                node1Parent->setLeft(node2);
                node2->setParent(node1Parent);
                
            }
            else{
                node1Parent->setRight(node2);
                node2->setParent(node1Parent);
            }
            node1->setParent(node2);

        }
        node1->setLeft(node2Left);
        node1->setRight(node2Right);
        if (node2 == node1Left){
            node2->setLeft(node1);
            node2->setRight(node1Right);
            if (node1Right != nullptr){
                node1Right->setParent(node2);
            }
        } 
        else{
            node2->setLeft(node1Left);
            node2->setRight(node1);
            if (node1Left != nullptr){
                node1Left->setParent(node2);
            }
        }
    }
    //case 3:
    if (node2 == node1Parent){
        switchNodesLocation(node2, node1);
    }
    //case 4:
    if (node1Parent == node2Parent){
        if (node1Parent == nullptr && node2Parent ==nullptr){
            return StatusType::FAILURE;
        }
        node1->setLeft(node2Left);
        node1->setRight(node2Right);
        node2->setLeft(node1Left);
        node2->setRight(node1Right);
    }
    T temp;
    
    temp = node1->data_m;
    node1->data_m=node2->data_m;
    node2->data_m=temp;
    
    int tempHeight = node1->getHeight();
    node1->setHeight(node2->getHeight());
    node2->setHeight(tempHeight);


    return StatusType::SUCCESS;
}
// the function ~Tree() will free all the memory allocated for the tree.
//the complexity of the ~Tree() must be O(n) where n is the number of nodes in the tree.
template<class T>
void Tree<T>::DestroyRecursive(Node<T>* node)
{
    if (node)
    {
        DestroyRecursive(node->getLeft());
        DestroyRecursive(node->getRight());
        delete node;
    }
}
template<class T>
Tree<T>::~Tree(){
    DestroyRecursive(root_m);
}





template<class T>
output_t<Node<T>*> Tree<T>::insert(T data){
    Node<T>* newNode = new Node<T>(data);
    if(newNode == nullptr){
        return output_t<Node<T>*>(StatusType::ALLOCATION_ERROR);
    }
    if(root_m == nullptr){
        root_m = newNode;
        return output_t<Node<T>*>(newNode);
    }
    Node<T>* current = root_m;
    Node<T>* parent = nullptr;
    while(current != nullptr){
        parent = current;
        if(data < current->getData()){
            current = current->getLeft();
        }
        else if(data > current->getData()){
            current = current->getRight();
        }
        else{
            delete newNode;
            return output_t<Node<T>*>(StatusType::FAILURE);
        }
    }
    newNode->setParent(parent);
    if(data < parent->getData()){
        parent->setLeft(newNode);
    }
    else{
        parent->setRight(newNode);
    }
    StatusType status = rebalance(newNode);
    if (status != StatusType::SUCCESS){
        return output_t<Node<T>*>(status);
    }
    return output_t<Node<T>*>(newNode);
}
// the time complexity of the insert function is O(log(n)).

template<class T>
StatusType Tree<T>::remove(T data){
    output_t<Node<T>*> output= find(data);
    if (output.status() != StatusType::SUCCESS){
        return output.status();
    }
    Node<T>* node = output.ans();
    if(node == nullptr){
        return StatusType::FAILURE;
    }
    Node<T>* parent = node->getParent();
    if(node->getLeft() == nullptr && node->getRight() == nullptr){
        if(parent == nullptr){
            setRoot(nullptr);
        }
        else if(parent->getLeft() == node){
            parent->setLeft(nullptr);
        }
        else{
            parent->setRight(nullptr);
        }
        T temp = T();
        temp = node->data_m;
        delete node;
        return rebalance(parent);
    }
    else if(node->getLeft() == nullptr || node->getRight() == nullptr){
        Node<T>* child = node->getLeft() == nullptr ? node->getRight() : node->getLeft();
        if(parent == nullptr){
            setRoot(child);
        }
        else if(parent->getLeft() == node){
            parent->setLeft(child);
        }
        else{
            parent->setRight(child);
        }
        child->setParent(parent);
        T temp;
        temp = node->data_m;
        
        delete node;
        return rebalance(parent);
    }
    else{
        Node<T>* successor = findSuccessor(node);
        T temp = T();
        temp = successor->data_m;
        switchNodesLocation(node, successor);
        remove(temp);
        T temp2 = T();
        temp2 =temp;
        successor->data_m = temp2;
        return StatusType::SUCCESS;
    }
}

    
// the time complexity of the remove function is O(log(n)).
template<class T>
output_t<Node<T>*> Tree<T>::find(T data){
    Node<T>* current = root_m;
    while(current != nullptr){
        if(current->data_m == data){
            return output_t<Node<T>*>(current);
        }
        if(current->data_m > data){
            current = current->getLeft();
        }
        else if(current->data_m < data ){
            current = current->getRight();
        }
        else{
            return output_t<Node<T>*>(current);
        }
    }
    return output_t<Node<T>*>(StatusType::FAILURE);
}

// the time complexity of the find function is O(log(n)).
template<class T>
Node<T>* Tree<T>::findMin(Node<T>* node){
    Node<T>* curr = node;
    while(curr->getLeft() != nullptr){
        curr = curr->getLeft();
    }
    return curr;
}

// the time complexity of the findMin function is O(log(n)).
template<class T>
Node<T>* Tree<T>::findMax(Node<T>* node){
    Node<T>* curr = node;
    if(curr == nullptr){
        return nullptr;
    }
    while(curr->getRight() != nullptr){
        curr = curr->getRight();
    }
    return curr;
}
// the time complexity of the findMax function is O(log(n)).

template<class T>
StatusType Tree<T>::RRrotation(Node<T>* node){
    Node<T>* parent = node->getParent();
    Node<T>* right = node->getRight();
    Node<T>* rightLeft = right->getLeft();
    if(parent == nullptr){
        setRoot(right);
    }
    else if(parent->getLeft() == node){
        parent->setLeft(right);
    }
    else{
        parent->setRight(right);
    }
    right->setParent(parent);
    right->setLeft(node);
    node->setParent(right);
    node->setRight(rightLeft);
    if(rightLeft != nullptr){
        rightLeft->setParent(node);
    }
    updateHeight(node);
    updateHeight(right);
    return StatusType::SUCCESS;
}
// the time complexity of the RRrotation function is O(1).

template<class T>
StatusType Tree<T>::LLrotation(Node<T>* node){
    Node<T>* parent = node->getParent();
    Node<T>* left = node->getLeft();
    Node<T>* leftRight = left->getRight();
    if(parent == nullptr){
        setRoot(left);
    }
    else if(parent->getLeft() == node){
        parent->setLeft(left);
    }
    else{
        parent->setRight(left);
    }
    left->setParent(parent);
    left->setRight(node);
    node->setParent(left);
    node->setLeft(leftRight);
    if(leftRight != nullptr){
        leftRight->setParent(node);
    }
    updateHeight(node);
    updateHeight(left);
    return StatusType::SUCCESS;
}
// the time complexity of the LLrotation function is O(1).

template<class T>
StatusType Tree<T>::LRrotation(Node<T>* node){
    Node<T>* left = node->getLeft();
    RRrotation(left);
    LLrotation(node);
    return StatusType::SUCCESS;
}
// the time complexity of the LRrotation function is O(1).

template<class T>
StatusType Tree<T>::RLrotation(Node<T>* node){
    Node<T>* right = node->getRight();
    LLrotation(right);
    RRrotation(node);
    return StatusType::SUCCESS;
}
// the time complexity of the RLrotation function is O(1).

template<class T>
StatusType Tree<T>::rebalance(Node<T>* node){
    if(node == nullptr){
        return StatusType::SUCCESS;
    }
    int balance = getBalance(node);
    if(balance > 1){
        if(getBalance(node->getLeft()) >= 0){
            LLrotation(node);
        }
        else{
            LRrotation(node);
        }
    }
    else if(balance < -1){
        if(getBalance(node->getRight()) <= 0){
            RRrotation(node);
        }
        else{
            RLrotation(node);
        }
    }
    else{
        updateHeight(node);
    }
    return rebalance(node->getParent());
}


template<class T>
int Tree<T>::getBalance(Node<T>* node){
    if(node == nullptr){
        return 0;
    }
    if (node->getLeft() == nullptr && node->getRight() == nullptr){
        return 0;
    }
    else if(node->getLeft() == nullptr){
        return -(node->getRight()->getHeight());
    }
    else if(node->getRight() == nullptr){
        return node->getLeft()->getHeight();
    }
    else{
        return node->getLeft()->getHeight() - node->getRight()->getHeight();
    }
}

template<class T>
int Tree<T>::getBalanceFactor(Node<T>* node){
    if(node == nullptr){
        return 0;
    }
    int leftHeight = node->getLeft() == nullptr ? 0 : node->getLeft()->getHeight();
    int rightHeight = node->getRight() == nullptr ? 0 : node->getRight()->getHeight();
    return leftHeight - rightHeight;
}
// the time complexity of the getBalanceFactor function is O(1).
template<class T>
StatusType Tree<T>::updateHeight(Node<T>* node){
    if(node == nullptr){
        return StatusType::SUCCESS;
    }
    int leftHeight = node->getLeft() == nullptr ? 0 : node->getLeft()->getHeight();
    int rightHeight = node->getRight() == nullptr ? 0 : node->getRight()->getHeight();
    int temp = leftHeight > rightHeight ? leftHeight : rightHeight;
    node->setHeight(temp + 1);
    return StatusType::SUCCESS;
}
// the time complexity of the updateHeight function is O(1).

template<class T>
Node<T>* findParent(Node<T>* node){
    if(node == nullptr){
        return nullptr;
    }
    return node->getParent();
}

template<class T>
Node<T>* Tree<T>::findSuccessor(Node<T>* node){
    if(node == nullptr){
        return nullptr;
    }
    if(node->getRight() != nullptr){
        return findMin(node->getRight());
    }
    Node<T>* parent = node->getParent();
    while(parent != nullptr && node == parent->getRight()){
        node = parent;
        parent = parent->getParent();
    }
    return parent;
}

template<class T>
Node<T>* Tree<T>::findPredecessor(Node<T>* node){
    if(node == nullptr){
        return nullptr;
    }

    if (node->left_m != nullptr){
        
        return findMax(node->getLeft());
    }
    
    Node<T>* parent = node->getParent();
    
    while(parent != nullptr && node == parent->getLeft()){
        node = parent;
        
        parent = node->getParent();
    }
    
    return parent;
}

template<class T>
Node<T>* findNext(Node<T>* node){
    if(node == nullptr){
        return nullptr;
    }
    if(node->getRight() != nullptr){
        return findMin(node->getRight());
    }
    Node<T>* parent = node->getParent();
    while(parent != nullptr && node == parent->getRight()){
        node = parent;
        parent = parent->getParent();
    }
    return parent;
}

// the time complexity of the findNext function is O(log(n)).

template<class T>
Node<T>* findPrev(Node<T>* node){
    if(node == nullptr){
        return nullptr;
    }
    if(node->getLeft() != nullptr){
        return findMax(node->getLeft());
    }
    Node<T>* parent = node->getParent();
    while(parent != nullptr && node == parent->getLeft()){
        node = parent;
        parent = parent->getParent();
    }
    return parent;
}

 #endif //WET1_AVL_H

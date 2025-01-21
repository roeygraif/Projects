#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <iostream>
#include <stdexcept>


using std::cout;
using std::endl;
using std::string;


namespace mtm {

//    each element in the sortedlist is of type Node<T>*
    template<class T>
    class Node {
    private:
        T data;
        Node *next;

    public:
//        c'tor
        Node<T>(T &data);
//        c'tor
        Node<T>(const T &data);
//        copy c'tor
        Node<T>(const Node<T> &node);
//        data getter
        T &getData();
//        sets data
        void setData(T &new_data);
//        next getter
        Node<T> *getNext();
//        next setter
        void setNext(Node<T> *new_next);

        Node<T> &operator=(Node<T> &node);
        Node<T> &operator=(const Node<T> &node);

        Node<T> &operator++();

        bool operator==(const Node<T> &node) const;
    };

    template<class T>
    Node<T>::Node(const Node<T> &node) {
        this->data = node.getData();
        this->next = node.getNext();
    }

    template<class T>
    Node<T>::Node(T &data): data(data), next(NULL) {}

    template<class T>
    Node<T>::Node(const T &data): data(data), next(NULL) {}

    template<class T>
    T &Node<T>::getData() {
        return this->data;
    }


    template<class T>
    void Node<T>::setData(T &new_data) {
        this->data = new_data;
    }


    template<class T>
    Node<T> *Node<T>::getNext() {
        return this->next;
    }


    template<class T>
    void Node<T>::setNext(Node<T> *new_next) {
        this->next = new_next;
    }


    template<class T>
    Node<T> &Node<T>::operator=(Node<T> &node) {
        this->data = node.data;
        this->next = node.next;
        return *this;
    }

    template<class T>
    Node<T> &Node<T>::operator=(const Node<T> &node) {
        this->data = node.data;
        this->next = node.next;
        return *this;
    }

    template<class T>
    Node<T> &Node<T>::operator++() {
        Node<T> new_node(this->next);
        this->next = new_node.next;
        this->data = new_node.data;
        return *this;
    }

    template<class T>
    bool Node<T>::operator==(const Node<T> &node) const {
        return (this->data == node.data && this->next == node.next);
    }


    template<class T>
    class SortedList {
        private:
            Node<T> *head;
            int list_length;

            void clear();

        public:
            class const_iterator {
                private:
                    Node<T> *node;
                    const SortedList<T> *list;

                    const_iterator(Node<T> *node, const SortedList<T> *list):
                        node(node), list(list) {}

                    friend class SortedList<T>;

                public:
                    const_iterator(const const_iterator &iter);

                    const_iterator &operator=(const const_iterator &iter);

                    const_iterator &operator++();

                    const const_iterator operator++(int);

                    bool operator==(const const_iterator &itr) const;

                    const T &operator*() const;
            };

            SortedList();

            ~SortedList();

            SortedList(const SortedList<T> &lst);

            SortedList& operator=(const SortedList<T> &lst);

            void insert(const T &element);

            void remove(const_iterator iter);

            int length();

            template<typename Pred>
            SortedList filter(Pred p) {
                SortedList<T> filtered_list;
                for (const_iterator it = this->begin(); !(it == this->end()); ++it) {
                    if (p(*it))
                        filtered_list.insert(*it);
                }
                return filtered_list;
            }

            template<typename Pred>
            SortedList apply(Pred p) {
                SortedList<T> applied_list;

                Node<T>* iter = this->head;
                for (int i = 0; i < this->list_length; i++) {
                    applied_list.insert(p(iter->getData()));
                    iter = iter->getNext();
                }

                return applied_list;
            }

            const_iterator begin() const {
                const_iterator begin_iter(this->head, this);
                return begin_iter;
            }

            const_iterator end() const {
                const_iterator iter = this->begin();
                for (int i = 0; i < this->list_length; i++) {
                    ++iter;
                }
                return iter;
            }
    };

    template<class T>
    void SortedList<T>::clear(){
        int len = this->list_length;
        for(int i = 0; i < len; i++){
            this->remove(this->begin());
        }
    }

    template<class T>
    SortedList<T>::SortedList(): head(NULL), list_length(0) {}

    template<class T>
    SortedList<T>::~SortedList(){
        this->clear();
    }

    template<class T>
    SortedList<T>::SortedList(const SortedList<T> &lst) {
        this->head = NULL;
        this->list_length = 0;

        Node<T> *iter_ptr = lst.head;
        while (iter_ptr != NULL) {
            this->insert(iter_ptr->getData());
            iter_ptr = iter_ptr->getNext();
        }
    }

    template<class T>
    SortedList<T> &SortedList<T>::operator=(const SortedList<T> &lst) {
        if(this->head != NULL && this->list_length != 0){
            this->clear();
        }

        const_iterator it = lst.begin();
        for(int i = 0; i < lst.list_length; i++){
            this->insert(it.node->getData());
            ++it;
        }

        return *this;
    }

    template<class T>
    void SortedList<T>::insert(const T &element) {
        Node<T> *new_node = new Node<T>(element);

        if (this->head == NULL) {
            this->head = new_node;
            this->list_length++;
            return;
        }

        if (element < this->head->getData()) {
            new_node->setNext(this->head);
            this->head = new_node;
            this->list_length++;
            return;
        }

        Node<T> *node_ptr = this->head;
        while (node_ptr->getNext() != NULL) {
            if (element < node_ptr->getNext()->getData()) {
                new_node->setNext(node_ptr->getNext());
                node_ptr->setNext(new_node);
                this->list_length++;
                return;
            }
            node_ptr = node_ptr->getNext();
        }

        node_ptr->setNext(new_node);//in brackets needs to be the pointer to new_node
        this->list_length++;
    }


    template<class T>
    void SortedList<T>::remove(typename SortedList<T>::const_iterator iter) {
        //assert same list
        if (iter.list != this || iter.node == NULL) {
            return;
        }

        Node<T> *node_ptr = this->head;
        if (node_ptr == NULL) {
            return;
        }

        if (this->head == iter.node) {
            this->head = iter.node->getNext();
            delete iter.node;
            this->list_length--;
            return;
        }

        while (node_ptr->getNext() != NULL) {
            if (node_ptr->getNext() == iter.node) {
                node_ptr->setNext(iter.node->getNext());
                delete iter.node;
                this->list_length--;
                return;
            }
            node_ptr = node_ptr->getNext();
        }
    }

    template<class T>
    int SortedList<T>::length() {
        return this->list_length;
    }


    template<class T>
    SortedList<T>::const_iterator::const_iterator(const const_iterator &iter) {
        this->node = iter.node;
        this->list = iter.list;
    }


    template<class T>
    typename SortedList<T>::const_iterator &SortedList<T>::const_iterator::operator=(const const_iterator &iter) {
        this->node = iter.node;
        this->list = iter.list;
        return *this;
    }


    template<class T>
    typename SortedList<T>::const_iterator &SortedList<T>::const_iterator::operator++() {
        if (this->node == NULL) {
            throw std::out_of_range("Attempt to iterate out of the list");
        }
        this->node = this->node->getNext();
        return *this;
    }


    template<class T>
    const typename SortedList<T>::const_iterator SortedList<T>::const_iterator::operator++(int) {
        const const_iterator result = *this;
        ++(*this);
        return result;
    }


    template<class T>
    bool SortedList<T>::const_iterator::operator==(const const_iterator &itr) const {
        return (this->node == itr.node);
    }


    template<class T>
    const T &SortedList<T>::const_iterator::operator*() const {
        return this->node->getData();
    }

}

#endif
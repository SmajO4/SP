#pragma once
#include <cstddef>
#include <utility>


template <class T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    template<class U>
    Node (U&& val) :
        data(std::forward<U>(val)),
        next(nullptr), prev(nullptr) {}
};

template <class T>
class List {
    private:
        Node<T>* head;
        Node<T>* tail;
        size_t size_;
    public:
        List();
        List(const List&);
        List(List&&);
        List& operator=(const List&);
        List& operator=(List&&);
        ~List();

        void clear();
        template <class U>
          void push_back(U&&);
        template <class U>
          void push_front(U&&);
        void pop_back();
        void pop_front();

        bool empty() const;
        size_t size() const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        class Iterator;
        Iterator begin();
        Iterator end();
        template <class U>
          Iterator insert(Iterator pos, U&& data);
        Iterator erase(Iterator pos);
        Iterator erase(Iterator beginIt, Iterator endIt);
        template <typename Predicate>
          void remove_if(Predicate pred);
        void reverse();
        List split_front(Iterator pos);

        class Iterator {
            private: 
                Node<T>* node;
                friend class List<T>;
            public:
                Iterator(Node<T>* n) : node(n) {}
                T& operator*() const;
                Iterator& operator++();
                Iterator operator++(int);
                Iterator& operator--();
                Iterator operator--(int);
                bool operator==(const Iterator&) const;
                bool operator!=(const Iterator&) const;
        };
};

template <class T>
List<T>::List() {
    head = tail = nullptr;     
    size_ = 0;
}

template <class T>
List<T>::List(const List& other) {
    head = tail = nullptr;
    size_ = 0;

    for (Node<T>* n = other.head ; n ; n = n -> next)
        this -> push_back(n -> data);
}

template <class T>
List<T>::List(List&& other) {
    head = other.head;
    tail = other.tail;
    size_ = other.size_;

    other.head = other.tail = nullptr;
    other.size_ = 0;
}

template <class T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this == &other) return *this;
    
    clear();
    for (Node<T>* n = other.head ; n ; n = n -> next)
        this -> push_back(n -> data);

    return *this;
}

template <class T>
List<T>& List<T>::operator=(List&& other) {
    if (this == &other) return *this;

    clear();
    head = other.head;
    tail = other.tail;
    size_ = other.size_;

    other.head = other.tail = nullptr;
    other.size_ = 0;

    return *this;
}

template <class T>
List<T>::~List() {
    this -> clear();
}

template <class T>
void List<T>::clear() {
    Node<T>* curr = head;
    while(curr) {
        Node<T>* next = curr -> next;
        delete curr;
        curr = next;
    }
    head = tail = nullptr;
    size_ = 0;
}

template <class T>
template <class U>
void List<T>::push_back(U&& newdata){
    Node<T>* newNode = new Node<T>(std::forward<U>(newdata));

    if (empty()) head = tail = newNode;

    else {
        newNode -> prev = tail;
        tail -> next = newNode;
        tail = newNode;
    }

    ++size_;
}

template <class T>
template <class U>
void List<T>::push_front(U&& newdata) {
    Node<T>* newNode = new Node<T>(std::forward<U>(newdata));
    
    if (empty()) head = tail = newNode;

    else {
        newNode -> next = head;
        head -> prev = newNode;
        head = newNode; 
    }

    ++size_;
}


template <class T>
void List<T>::pop_back() {
    if (empty()) return;

    Node<T>* nodeToPop = tail;
    tail = tail -> prev;

    if (tail) 
        tail -> next = nullptr;
    else
        head = nullptr;

    delete nodeToPop;
    --size_;
}

template <class T>
void List<T>::pop_front() {
    if (empty()) return;

    Node<T>* nodeToPop = head;
    head = head -> next;

    if (head)
        head -> prev = nullptr;
    else
        tail = nullptr;

    --size_;
}

template <class T>
bool List<T>::empty() const {
    return size_ == 0;
}

template <class T>
size_t List<T>::size() const {
    return size_;
}

template <class T>
T& List<T>::front() {
    if (empty()) return;
    return head -> data;
}

template <class T>
const T& List<T>::front() const {
    if (empty()) return;
    return head -> data;
}

template <class T>
T& List<T>::back() {
    if (empty()) return;
    return tail -> data;
}

template <class T>
const T& List<T>::back() const {
    if (empty()) return;
    return tail -> data;
}

template <class T>
typename List<T>::Iterator List<T>::begin() {
    return Iterator(head);
}

template <class T>
typename List<T>::Iterator List<T>::end() {
    return Iterator(nullptr);
}

template <class T>
template <class U>
typename List<T>::Iterator List<T>::insert(Iterator pos, U&& value) {
    if (pos.node == nullptr) {
        push_back(std::forward<U>(value));
        return Iterator(tail);
    }

    Node<T>* curr = pos.node;
    Node<T>* newNode = new Node<T>(std::forward<U>(value));

    newNode -> prev = curr -> prev;
    newNode -> next = curr;
    if (curr -> prev)
        curr -> prev -> next = newNode; 
    else
        head = newNode;
    curr -> prev = newNode;

    ++size_;
    return Iterator(newNode);
}

template <class T>
typename List<T>::Iterator List<T>::erase(Iterator pos) {
    if (!pos.node)
        return end();

    Node<T>* curr = pos.node;
    Iterator next{curr -> next};

    if (curr -> prev)
        curr -> prev -> next = curr -> next;
    else
        head = curr -> next;
    if (curr -> next)
        curr -> next -> prev = curr -> prev;
    else
        tail = curr -> prev;

    delete curr;
    --size_;
    return next;
}

template <class T>
typename List<T>::Iterator List<T>::erase(Iterator beginIt, Iterator endIt) {
    while (beginIt != endIt)
        beginIt = erase(beginIt);
    
    return endIt;
}

template <class T>
template <class Predicate>
void List<T>::remove_if(Predicate pred) {
    Iterator it = begin();

    while (it != end()) {
        Iterator curr = it++;
        if (pred(*curr))
            erase(curr); 
    }
}

template <class T>
void List<T>::reverse() {
    if (size_ <= 1) return;

    Iterator front = begin();
    Iterator back = --end();

    for (size_t i = 0 ; i < size_/2 ; ++i)
        std::swap(*front++, *back--);
}

template <class T>
List<T> List<T>::split_front(Iterator pos) {
    List<T> newList;
    if (!pos.node || pos.node == head) 
        return newList;

    Node<T>* splitNode = pos.node;
    newList.head = head;
    newList.tail = splitNode -> prev;
    newList.tail -> next = nullptr;

    head = splitNode;
    head -> prev= nullptr;

    Node<T>* curr = newList.head;
    newList.size_ = 0;

    while (curr) {
        ++newList.size_;
        curr = curr -> next;
    }

    size_ -= newList.size_;
    return newList;
}

template <class T>
T& List<T>::Iterator::operator*() const {
    return node -> data;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    node = node -> next;
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    Iterator tempIt = *this;
    ++(*this);
    return tempIt;
}

template <class T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
    node = node -> prev;
    return *this;
}

template <class T>
typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    Iterator tempIt = *this;
    node = node -> prev;
    return *this;
}

template <class T>
bool List<T>::Iterator::operator==(const Iterator& other) const {
    return node == other.node;
}

template <class T>
bool List<T>::Iterator::operator!=(const Iterator& other) const {
    return node != other.node;
}

#pragma once
#include <stdexcept>
#include <initializer_list>
#include <unordered_set>
#include "Training.h"

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int length;

    template<class U> friend struct LLHook;
public:
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(T* items, int count) : LinkedList() {
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }

    LinkedList(std::initializer_list<T> list) : LinkedList() {
        for (auto& element : list) {
            Append(element);
        }
    }

    LinkedList(const LinkedList<T>& other) : LinkedList() {
        Node* current = other.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }

    ~LinkedList() {
        Clear();
    }

    void Clear() {
        std::unordered_set<Node*> seen;
        Node* current = head;
        while (current && !seen.count(current)) {
            seen.insert(current);
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
        length = 0;
    }

    T& GetFirst() {
        if (length == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        return head->data;
    }

    const T& GetFirst() const {
        if (length == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        return head->data;
    }

    T& GetLast() {
        if (length == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        return tail->data;
    }

    const T& GetLast() const {
        if (length == 0) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        return tail->data;
    }

    T& Get(int index) {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= length) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    const T& Get(int index) const {
        return const_cast<LinkedList*>(this)->Get(index);
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (endIndex < 0 || startIndex >= length || endIndex >= length || startIndex > endIndex) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        LinkedList<T>* subList = new LinkedList<T>();
        Node* current = head;
        for (int i = 0; i < startIndex; i++) {
            current = current->next;
        }
        for (int i = startIndex; i <= endIndex; i++) {
            subList->Append(current->data);
            current = current->next;
        }
        return subList;
    }

    int GetLength() const {
        return length;
    }

    void Append(const T& item) {
        Node* newNode = new Node(item);
        if (length == 0) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        length++;
    }

    void RemoveAt(int index) {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 2);
        }
        if (index >= length) {
            throw MyException(ErrorType::OutOfRange, 3);
        }
        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
            length--;
            if (length == 0) {
                tail = nullptr;
            }
            return;
        }
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        Node* toDel = current->next;
        current->next = toDel->next;
        delete toDel;
        length--;
        if (index == length) {
            tail = current;
        }
    }

    void Prepend(const T& item) {
        Node* newNode = new Node(item);
        if (length == 0) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        length++;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0) {
            throw MyException(ErrorType::OutOfRange, 0);
        }
        if (index >= length) {
            throw MyException(ErrorType::OutOfRange, 1);
        }
        if (index == 0) {
            Prepend(item);
            return;
        } else if (index == length) {
            Append(item);
            return;
        }
        Node* newNode = new Node(item);
        Node* current = head;
        for (int i = 0; i < index - 1; i++) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        length++;
    }

    LinkedList<T>* Concat(LinkedList<T>* list) {
        if (!list || list->length == 0) {
            return this;
        }
        if (this->length == 0) {
            this->head = list->head;
            this->tail = list->tail;
            this->length = list->length;
        } else {
            this->tail->next = list->head;
            this->tail = list->tail;
            this->length += list->length;
        }
        list->head = nullptr;
        list->tail = nullptr;
        list->length = 0;
        return this;
    }

    void reverse() {
        Node* prev = nullptr;
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
        tail = head;
        if (tail) {
            while (tail->next) {
                tail = tail->next;
            }
        }
    }

    void MakeCycle(int idx) {
        if (idx < 0 || idx >= length || length == 0) {
            return;
        }
        Node* cycleStart = head;
        for (int i = 0; i < idx; ++i) {
            cycleStart = cycleStart->next;
        }
        tail->next = cycleStart;
    }

    void ReverseSmart() {
    if (length < 2) return;

    Node* slow = head;
    Node* fast = head;
    bool hasCycle = false;

    while (fast && fast->next){
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { hasCycle = true; break; }
    }

    if (!hasCycle){
        reverse();
        return;
    }
    
    Node* p1 = head;
    Node* p2 = slow;
    while (p1 != p2)
    {
        p1 = p1->next;
        p2 = p2->next;
    }
    Node* cycleStart = p1;
    Node* cycleTail = cycleStart;
    while (cycleTail->next != cycleStart){
        cycleTail = cycleTail->next;
    }
    cycleTail->next = nullptr;
    tail = cycleTail;

    reverse();
    tail->next = head;
}


    void makeCycle() {
        MakeCycle(0);
    }

    void smartReverse() {
        ReverseSmart();
    }

    T& Next(const T& value){
        Node* p = head;
        for (int i = 0; i < length; ++i) {
            if (p->data == value) {
                if (!p->next)
                    throw MyException(ErrorType::OutOfRange,3);
                return p->next->data;
            }
            p = p->next;
        }
        throw MyException(ErrorType::InvalidArg,6);
    }


    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            Clear();
            Node* current = other.head;
            while (current) {
                Append(current->data);
                current = current->next;
            }
        }
        return *this;
    }
};
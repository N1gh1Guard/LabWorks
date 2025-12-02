#pragma once
#include "Exception.h"

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

public:
    LinkedList() : head(nullptr), tail(nullptr), length(0) {}

    LinkedList(T* items, int count) : LinkedList() {
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr), length(0) {
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
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
        length = 0;
    }

    T& GetFirst() {
        if (length == 0) {
            throw MyException(3, "Sequence is empty");
        }
        return head->data;
    }

    const T& GetFirst() const {
        if (length == 0) {
            throw MyException(3, "Sequence is empty");
        }
        return head->data;
    }

    T& GetLast() {
        if (length == 0) {
            throw MyException(3, "Sequence is empty");
        }
        return tail->data;
    }

    const T& GetLast() const {
        if (length == 0) {
            throw MyException(3, "Sequence is empty");
        }
        return tail->data;
    }

    T& Get(int index) {
        if (index < 0) {
            throw MyException(2, "Index out of range");
        }
        if (index >= length) {
            throw MyException(2, "Index out of range");
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
            throw MyException(2, "Index out of range");
        }
        if (endIndex < 0 || startIndex >= length || endIndex >= length || startIndex > endIndex) {
            throw MyException(2, "Index out of range");
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
            throw MyException(2, "Index out of range");
        }
        if (index >= length) {
            throw MyException(2, "Index out of range");
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
            throw MyException(2, "Index out of range");
        }
        if (index > length) {
            throw MyException(2, "Index out of range");
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
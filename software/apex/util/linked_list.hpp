#pragma once


// This file defines a simple templated linked list
template <typename T>
class LinkedList {
public:
    LinkedList(): head(nullptr),
                  tail(nullptr),
                  length(0) {};

    bool is_empty() {
        return length == 0;
    }

    void append(T* data) {
        Node* node = new Node(data);

        if (is_empty()) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = tail->next;
        }

        length++;
    }

    T* pop() {
        if (is_empty()) {
            return nullptr;
        }

        T* data = head->data;
        
        Node* temp = head;
        head = head->next;

        delete temp; // Every "new" needs a "delete"
        return data;
    }

    T* remove(bool (*query)(T data)) {
        Node* prev;
        Node* curr = head;

        while (curr != nullptr) {
            if (query(*curr->data)) {
                break; // Stop on the first match
            }
            prev = curr;
            curr = curr->next;
        }

        if (curr != nullptr) { // We found a match
            T* data = curr->data;

            if (head == curr) {
                head = curr->next;
            } else if (tail == curr) {
                tail = prev;
                prev->next = nullptr;  
            } else {
                prev->next = curr->next;
            }
            
            length--;
            delete curr;
            return data;
        }
        return nullptr;
    }

private:

    // This is a class that belongs to the linked list
    class Node {
    public:
        Node(T* data): data(data), next(nullptr) {}

        T* data;
        Node* next;
    };


    Node* head;
    Node* tail;
    size_t length;
};
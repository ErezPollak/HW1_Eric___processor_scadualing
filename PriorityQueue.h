#pragma once
#include <memory>
#include <iostream>

template <typename T>
class PriorityQueue
{
public:
    PriorityQueue();
    ~PriorityQueue();

    //Highest priority item is dequeued first. If the same priority is used, the earlier queued item is dequeued first
    void dequeue();
    void enqueue(int priority, T item);

    bool isEmpty();

    T& first() const;

private:

    struct Node
    {
        Node* next;
        Node* prev;
        T data;
        int priority;

        Node(T data, int priority, Node* next, Node* prev)
                :data(data)
                ,priority(priority)
                ,next(next)
                ,prev(prev)
        {}
    };

    Node* head;
    Node* tail;
};


//
// Created by pwlqr on 15/03/2022.
//

template <typename T>
PriorityQueue<T>::PriorityQueue()
        :head(nullptr)
        ,tail(nullptr)
{}


template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
    while (tail != nullptr)
        dequeue();
}


template <typename T>
void PriorityQueue<T>::dequeue()
{
    if (tail == nullptr)
        throw std::runtime_error("Nothing in queue");

    tail = tail->next;

    if(tail != nullptr)
        tail->prev = nullptr;

    if(tail == nullptr)
        head = nullptr;
}

template <typename T>
void PriorityQueue<T>::enqueue(int priority, T item)
{
    Node* newNode = new Node(item, priority, nullptr, nullptr);

    if (head == nullptr)
    {
        head = tail = newNode;
        return;
    }
    Node* curr = nullptr;
    for (curr = head; curr != nullptr && priority < curr->priority; curr = curr->prev);

    if (curr == nullptr)        //We are at the end of queue, the priority I want to insert must be the highest priority in the queue, we didn't find any lower priority
    {
        newNode->next = tail;
        tail->prev = newNode;
        newNode->prev = nullptr;
        tail = newNode;
    }
    else                        //curr now points to the Node that is just barely in front of newNode. curr is either higher priority than newNode, or same priority but inserted earlier
    {
        newNode->prev = curr;
        newNode->next = curr->next;

        if(newNode->next != nullptr)
            newNode->next->prev = newNode;

        newNode->prev->next = newNode;

        if (head == tail || priority >= head->priority)
            head = newNode;
    }
}

template <typename T>
T& PriorityQueue<T>::first() const
{
    if (tail == nullptr)
        throw std::runtime_error("Nothing in queue");

    return tail->data;
}

//
//template <typename T>
//T& PriorityQueue<T>::last() const
//{
//    if (head == nullptr)
//        throw std::runtime_error("Nothing in queue");
//
//    return head->data;
//}

template<typename T>
bool PriorityQueue<T>::isEmpty() {
    return this->head == nullptr || this->tail == nullptr;
}

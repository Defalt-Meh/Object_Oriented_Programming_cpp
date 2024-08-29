#include "DynQueue.h"

Queue::Queue() : front(nullptr), rear(nullptr), count(0) {}

Queue::~Queue() {
    while (!isEmpty()) {
        dequeue();
    }
}

void Queue::enqueue(const District& district) {
    Node* newNode = new Node(district);
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    ++count;
}

District Queue::dequeue() {
    if (isEmpty()) throw std::out_of_range("Queue is empty");

    Node* temp = front;
    District data = front->data;
    front = front->next;
    delete temp;
    --count;

    if (isEmpty()) {
        rear = nullptr;
    }
    
    return data;
}

bool Queue::isEmpty() const {
    return front == nullptr;
}

int Queue::size() const {
    return count;
}

#ifndef DYNQUEUE_H
#define DYNQUEUE_H

#include <iostream>
#include<string>
#include<sstream>
#include <vector>
#include <mutex>
#include <cmath>
#include<iomanip>
#include <algorithm>
#include <thread>
#include <functional>
#include <fstream>



// Define a structure to hold district data
struct District {
    int id;
    float median_age;         // Assume this is the age of the house
    float avg_room_count;
    float avg_bedroom_count;
    float latitude;
    float longitude;
    float median_price;       // New field for median price
};

class Queue {
public:
    Queue();
    ~Queue();
    void enqueue(const District& district);
    District dequeue();
    bool isEmpty() const;
    int size() const;

private:
    struct Node {
        District data;
        Node* next;
        Node(const District& d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* front;
    Node* rear;
    int count;
};

#endif // QUEUE_H

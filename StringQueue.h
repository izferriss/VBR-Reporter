#pragma once
#include <iostream>
#include <string>


using namespace std;

class StringQueue
{
public:
	int front, rear;
	unsigned capacity;
	string* arr;

	StringQueue();
	~StringQueue();
	bool isFull();
	bool isEmpty();
	void enqueue(string& data);
	void dequeue();
	void print();
	void printFront();
	bool doesContain(string term);
};
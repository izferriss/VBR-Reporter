#pragma once
#include <iostream>
#include <string>
#include "TimeClass.h"

using namespace std;

class TimeQueue
{
public:
	int front, rear;
	unsigned capacity;
	TimeClass* arr;

	TimeQueue();
	~TimeQueue();
	bool isFull();
	bool isEmpty();
	void enqueue(TimeClass &data);
	void dequeue();
	void print();
	void printFront();
	bool doesContain(string term);
};

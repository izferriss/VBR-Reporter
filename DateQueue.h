#pragma once
#include <iostream>
#include "Date.h"
#include "Time.h"
#include "TimeQueue.h"

using namespace std;

class DateQueue
{
public:
	int front, rear;
	unsigned capacity;
	Date* arr;

	DateQueue();
	~DateQueue();
	bool isFull();
	bool isEmpty();
	void enqueue(Date data);
	void dequeue();
	void print();
	void printFront();
	bool doesContain(string term);
};
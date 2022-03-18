#include "StringQueue.h"
#include "Constants.h"

using namespace std;

StringQueue::StringQueue()
{
	front = rear = -1;
	capacity = MAX_DATES;
	arr = new string[MAX_DATES];
}

StringQueue::~StringQueue()
{
	delete[] arr;
	arr = nullptr;
}

bool StringQueue::isFull()
{
	return (front == 0 && rear == capacity - 1);
}

bool StringQueue::isEmpty()
{
	return (front == -1);
}

void StringQueue::enqueue(string& data)
{
	if (isFull())
	{
		cout << "Queue is full!" << endl;
	}
	else
	{
		if (front == -1)
		{
			front = 0;
		}

		rear++;

		arr[rear] = data;

	}
}

void StringQueue::dequeue()
{

	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}
	else
	{
		for (int i = 0; i < rear; i++)
		{
			arr[i] = arr[i + 1];
		}

		arr[rear] = "";
		rear--;
	}
}

void StringQueue::print()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}

	for (int i = front; i < rear; i++)
	{
		cout << arr[i] << endl;
	}
}

void StringQueue::printFront()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}

	cout << arr[front] << endl;

}

bool StringQueue::doesContain(string term)
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
		return false;
	}
	else
	{
		for (int i = 0; i < int(capacity); i++)
		{
			if (term == arr[i])
			{
				return true;
			}
		}
	}
	return false;
}
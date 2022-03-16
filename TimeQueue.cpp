#include "TimeQueue.h"

const int MAX_TIMES = 24;

using namespace std;


TimeQueue::TimeQueue()
{
	front = rear = -1;
	capacity = MAX_TIMES;
	arr = new TimeClass[MAX_TIMES];
}

TimeQueue::~TimeQueue()
{
	delete[] arr;
	arr = nullptr;
}

bool TimeQueue::isFull()
{
	return (front == 0 && rear == capacity -1);
}

bool TimeQueue::isEmpty()
{
	return (front == -1);
}

void TimeQueue::enqueue(TimeClass &data)
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
		arr[rear].lineNum = data.lineNum;
		arr[rear].mTime = data.mTime;

	}
}

void TimeQueue::dequeue()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}
	else
	{
		for (int i = 0; i < rear; i++)
			{
				arr[i].lineNum = arr[i + 1].lineNum;
				arr[i].mTime = arr[i + 1].mTime;
			}
		rear--;
	}
}

void TimeQueue::print()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}

	for (int i = front; i < rear; i++)
	{
		cout << arr[i].mTime << endl;
	}
}

void TimeQueue::printFront()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}

	cout << arr[front].mTime << endl;

}

bool TimeQueue::doesContain(string term)
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
		return false;
	}
	else
	{
		for (int i = 0; i < rear; i++)
		{
			if (term == arr[i].mTime)
			{
				return true;
			}
		}
	}
	return false;
}

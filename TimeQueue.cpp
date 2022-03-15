#include "TimeQueue.h"

const int MAX_TIMES = 24;

using namespace std;


TimeQueue::TimeQueue()
{
	front = rear = 0;
	capacity = MAX_TIMES;
	arr = new TimeClass[MAX_TIMES];
}

TimeQueue::~TimeQueue()
{
	delete[] arr;
}

bool TimeQueue::isFull()
{
	return (capacity == rear);
}

bool TimeQueue::isEmpty()
{
	return (front == rear);
}

void TimeQueue::enqueue(TimeClass data)
{
	if (isFull())
	{
		cout << "Queue is full!" << endl;
	}
	else
	{
		arr[rear].lineNum = data.lineNum;
		arr[rear].mTime = data.mTime;
		rear++;
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
		for (int i = front; i < rear - 1; i++)
		{
			arr[i] = arr[i + 1];
		}
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

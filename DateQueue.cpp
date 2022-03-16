#include "DateQueue.h"

const int MAX_DATES = 14;

using namespace std;

DateQueue::DateQueue()
{
	front = rear = 0;
	capacity = MAX_DATES;
	arr = new Date[MAX_DATES];
}

DateQueue::~DateQueue()
{
	delete[] arr;
}

bool DateQueue::isFull()
{
	return (capacity == rear);
}

bool DateQueue::isEmpty()
{
	return (front == rear);
}

void DateQueue::enqueue(Date &data)
{
	if (isFull())
	{
		cout << "Queue is full!" << endl;
	}
	else
	{
		arr[rear].mDate = data.mDate;
		arr[rear].timeArr.enqueue(data.timeArr.arr[data.timeCount - 1]);
		arr[rear].timeCount++;
		rear ++;
	}
}

void DateQueue::dequeue()
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

		rear--;
	}
}

void DateQueue::print()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}

	for (int i = front; i < rear; i++)
	{
		cout << arr[i].mDate << endl;
	}
}

void DateQueue::printFront()
{
	if (isEmpty())
	{
		cout << "Queue is empty!" << endl;
	}

	cout << arr[front].mDate << endl;

}

bool DateQueue::doesContain(string term)
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
			if (term == arr[i].mDate)
			{
				return true;
			}
		}
	}
	return false;
}

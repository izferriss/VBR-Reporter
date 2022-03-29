#include "DateQueue.h"
#include "Constants.h"

using namespace std;

DateQueue::DateQueue()
{
	front = rear = -1;
	capacity = MAX_DATES;
	arr = new Date[MAX_DATES];
}

DateQueue::~DateQueue()
{
	delete[] arr;
	arr = nullptr;
}

bool DateQueue::isFull()
{
	return (front == 0 && rear == capacity - 1);
}

bool DateQueue::isEmpty()
{
	return (front == -1);
}

void DateQueue::enqueue(Date &data)
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

		arr[rear].mDate = data.mDate;
		for (int i = 0; i < data.timeCount; i++)
		{
			arr[rear].timeArr.enqueue(data.timeArr.arr[i]);
			arr[rear].timeCount = i + 1;
		}
		

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
		for (int i = 0; i < rear; i++)
		{
			arr[i].mDate = arr[i + 1].mDate;

			for (int j = 0; j < arr[i + 1].timeCount; j++)
			{
				arr[i].timeArr.arr[j].lineNum = arr[i + 1].timeArr.arr[j].lineNum;
				arr[i].timeArr.arr[j].mTime = arr[i + 1].timeArr.arr[j].mTime;
			}
			arr[i].timeCount = arr[i + 1].timeCount;
		}
		arr[rear].mDate = "";
		for (int i = 0; i < arr[rear].timeCount; i++)
		{
			arr[rear].timeArr.dequeue();
		}
		arr[rear].timeCount = 0;
		rear--;
		if (rear == -1)
		{
			front = -1;
		}
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
		for (int i = 0; i < int(capacity); i++)
		{
			if (term == arr[i].mDate)
			{
				return true;
			}
		}
	}
	return false;
}

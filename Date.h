#pragma once
#include<string>
#include "TimeClass.h"
#include "TimeQueue.h"

using namespace std;

class Date
{
public:
	string mDate = "";
	int timeCount = -1;
	TimeQueue timeArr;
	Date();
	~Date();
};


#pragma once
#include <string>
#include "TimeQueue.h"

using namespace std;

class Date
{
public:
	string mDate = "";
	int timeCount = 0;
	TimeQueue timeArr;
	Date() {};
	~Date() {};
};


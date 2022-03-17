#pragma once
#include <string>
#include "DateQueue.h"

using namespace std;

class Job
{
public:
	string name;
	int dateCount;
	DateQueue dateArr;
	Job() {};
	~Job() {};
};


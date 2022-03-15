#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "Job.h"
#include "Date.h"
#include "TimeClass.h"
#include "TimeQueue.h"
#include "DateQueue.h"

//Constants
const int MAX_RECORDS = 1000;
const int MAX_DATES = 14;
const int MAX_TIMES = 24;

using namespace std;

//Variables
string path = "I:\\Temp\\PSTest-Ivan\\main_record.csv";				//file path ("\\" translates to "\" at run time, otherwise error)
ifstream din(path);													//opens file for reading
string sLine;														//placeholder string for reading an entire line of the csv
size_t q1, q2, q3, q4, comma, dateSpace;							//position placeholders for string operations (q = quotation)
string sJobName, sDate, sTime;										//placeholder strings for debugging
int numUniqueJobs = 0;												//keeps track of the number of elements in jobArr
int lineCount = 0;

Date newDate = Date();
TimeClass newTime = TimeClass();

Job jobArr[MAX_RECORDS];

//Function declarations
void printAll();

int main()
{
	//If file is open
	if (din.is_open())
	{
		getline(din, sLine);	//necessary action, but ignorable data; reads in headers
		lineCount++;
		getline(din, sLine);	//necessary action, but ignorable data; reads in blank line
		lineCount++;

		//Read in the rest of the file one line at a time until the end of the file
		while (din.good())
		{
			//Read in one line from file
			getline(din, sLine);
			lineCount++;

			//Find positions of all delimiters and extra characters in order from left to right
			//(q1, q2, comma, q3, dateSpace, q4)
			q1 = sLine.find("\"");
			q2 = sLine.find("\"", q1 + 1);
			comma = sLine.find(",", q2 + 1);
			q3 = sLine.find("\"", comma + 1);
			dateSpace = sLine.find(" ", q3 + 1);
			q4 = sLine.find("\"", q3 + 1);

			//Isolates JobName
			sJobName = sLine.substr(q1 + 1, q2 - q1 - 1);
			//TESTING: cout << sJobName << endl;

			//Isolates date
			sDate = sLine.substr(q3 + 1, dateSpace - q3 - 1);
			//TESTING: cout << sDate << endl;

			//Isolates time
			sTime = sLine.substr(dateSpace + 1, q4 - dateSpace - 1);
			//TESTING: cout << sTime << endl;

			//jobArr is empty
			if (numUniqueJobs == 0)
			{
				//Assign job name and initialize dateCount
				jobArr[numUniqueJobs].name = sJobName;
				jobArr[numUniqueJobs].dateCount = 0;

				//Assign time and initialize elements
				newTime.mTime = sTime;
				newTime.lineNum = lineCount;
				
				//Assign date and initialize elements
				newDate.mDate = sDate;
				newDate.timeCount = 0;

				//Assign time to newDate's timeArr
				newDate.timeArr.enqueue(newTime);

				//Increment count to reflect changes
				newDate.timeCount++;

				//Assign date to jobArr[x]'s dateArr
				jobArr[numUniqueJobs].dateArr.enqueue(newDate);

				//Increment count to reflect changes
				jobArr[numUniqueJobs].dateCount++;

				//Increment count to reflect changes
				numUniqueJobs++;
			}
			//jobArr contains elements
			else
			{
				//Loop through jobArr
				for (int i = 0; i < numUniqueJobs; i++)
				{
					//If jobName matches
					if (jobArr[i].name == sJobName)
					{
						//If job has date already
						if (jobArr[i].dateArr.doesContain(sDate))
						{
							//loop through jobArr's dateArr
							for (int j = 0; j < sizeof(jobArr[i].dateArr.arr); j++)
							{
								//if date matches
								if (jobArr[i].dateArr.arr[j].mDate == sDate)
								{
									//If job has time already
									if (jobArr[i].dateArr.arr[j].timeArr.doesContain(sTime))
									{
										//don't do anything
										break;
									}
									else
									{
										//Assign time and initialize elements
										newTime.mTime = sTime;
										newTime.lineNum = lineCount;

										//Add time to jobArr's dateArr's timeArr
										jobArr[i].dateArr.arr[j].timeArr.enqueue(newTime);
									}
								}
								else
								{
									//not found -- problem
									break;
								}
							}
								
						}
						//Job does not contain sDate yet
						else
						{
							//Assign time and initialize elements
							newTime.mTime = sTime;
							newTime.lineNum = lineCount;

							//Assign date and initialize elements
							newDate.mDate = sDate;
							newDate.timeCount = 0;

							//Assign time to newDate's timeArr
							newDate.timeArr.enqueue(newTime);

							//Increment count to reflect changes
							newDate.timeCount++;

							//Assign date to jobArr[x]'s dateArr
							jobArr[numUniqueJobs].dateArr.enqueue(newDate);

							//Increment count to reflect changes
							jobArr[numUniqueJobs].dateCount++;

							jobArr[i].dateArr.enqueue(newDate);
							jobArr[i].dateCount++;
						}
					}
					//Job is not yet in array
					else
					{
						//Assign job name and initialize dateCount
						jobArr[numUniqueJobs].name = sJobName;
						jobArr[numUniqueJobs].dateCount = 0;

						//Assign time and initialize elements
						newTime.mTime = sTime;
						newTime.lineNum = lineCount;

						//Assign date and initialize elements
						newDate.mDate = sDate;
						newDate.timeCount = 0;

						//Assign time to newDate's timeArr
						newDate.timeArr.enqueue(newTime);

						//Increment count to reflect changes
						newDate.timeCount++;

						//Assign date to jobArr[x]'s dateArr
						jobArr[numUniqueJobs].dateArr.enqueue(newDate);

						//Increment count to reflect changes
						jobArr[numUniqueJobs].dateCount++;

						//Increment count to reflect changes
						numUniqueJobs++;
					}
				}
			}
		}//end while

		//Closes file
		din.close();
	}
	//If file did not open
	else
	{
		//Display error
		cout << "Error opening file " + path << endl;
	}

	printAll();
	return 0;
}

void printAll()
{
	for (int i = 0; i < numUniqueJobs; i++)
	{
		cout << jobArr[i].name << endl;
		jobArr[i].dateArr.print();
		jobArr[i].dateArr.arr->timeArr.print();
	}
}
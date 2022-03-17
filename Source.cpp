#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Job.h"
#include "Date.h"
#include "TimeClass.h"
#include "TimeQueue.h"
#include "DateQueue.h"

//Constants
const int MAX_RECORDS = 1000;										//Number of unique jobs allowed
//const int MAX_DATES												//see DateQueue.cpp
//const int MAX_TIMES												//see TimeQueue.cpp

using namespace std;

//Variables
string path = "I:\\Temp\\PSTest-Ivan\\main_record.csv";				//use '\\' escape characters to infer '\' to compiler
ifstream din(path);													//opens file for reading
string sLine;														//placeholder string for reading an entire line of the csv
size_t q1, q2, q3, q4, comma, dateSpace;							//placeholder indexes for string operations (q = quotation)
string sJobName, sDate, sTime;										//placeholder strings for reading in file elements
int tempJobIndex, tempDateIndex, tempTimeIndex = -1;				//placeholder indexes used for adding new data to existing records

//Data Structures
Job jobArr[MAX_RECORDS];											//an array of Job classes - main data structure
Date newDate;														//a temporary Date class used to initialize variables before adding to jobArr
TimeClass newTime;													//a temporary TimeClass used to initalize variables before adding to Date class
int deleteLineArr[MAX_RECORDS];										//an array that keeps track of which lines to be deleted from the file we read from

//Counters
int numUniqueJobs = 0;												//keeps track of the number of elements in jobArr											
int lineCount = 0;													//keeps track of the number of lines read in
int numLinesToDelete = 0;											//keeps track of the number of lines needed to be deleted

//Flags
bool isFound = false;												//used in searching for exists jobs/dates/times
bool badFile = false;												//used to display error message if there are issues opening the file
bool maxJobs = false;												//used to display error message if MAX_RECORDS is not big enough for the dataset
bool dupLine = false;												//used to display error message if there are duplicate lines in the file (shouldn't happen ever)

//Declarations
void printDataToConsole();
void printDeleteLinesToConsole();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
			if (sizeof(jobArr) == 0 && numUniqueJobs < MAX_RECORDS)
			{
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

				//If jobArr's dateArr is full
				if (jobArr[numUniqueJobs].dateArr.isFull())
				{
					//Copy all time's line numbers from the front of the date queue into deleteLineArr
					for (int i = 0; i < jobArr[numUniqueJobs].dateArr.arr[0].timeCount; i++)
					{
						deleteLineArr[numLinesToDelete] = jobArr[numUniqueJobs].dateArr.arr[0].timeArr.arr[i].lineNum;
						numLinesToDelete++;
					}

					//Remove the first dateArr entry
					jobArr[numUniqueJobs].dateArr.dequeue();

					//Assign and count the new date into JobArr
					jobArr[numUniqueJobs].dateArr.enqueue(newDate);
					//Datecount remains the same

				}
				else
				{
					//Assign and count new date into JobArr
					jobArr[numUniqueJobs].dateArr.enqueue(newDate);
					jobArr[numUniqueJobs].dateCount++;
				}

				//Clean up temp variable
				newDate.timeArr.dequeue();
				newDate.timeCount = 0;

				//Increment count to reflect changes
				numUniqueJobs++;

			}
			//jobArr contains elements
			else
			{
				if (numUniqueJobs < MAX_RECORDS)
				{
					//Loop through jobArr
					for (int i = 0; i < numUniqueJobs; i++)
					{
						if (jobArr[i].name == sJobName)
						{
							tempJobIndex = i;
							isFound = true;
						}
					}
					if (isFound) //jobName is in jobArr
					{
						//reset flag to be reused
						isFound = false;

						//Loop through dateArr in found job to see if date already exists
						for (int i = 0; i < jobArr[tempJobIndex].dateCount; i++)
						{
							if (jobArr[tempJobIndex].dateArr.arr[i].mDate == sDate)
							{
								isFound = true;
								tempDateIndex = i;
							}
						}
						if (isFound) //sDate is in dateArr
						{
							//reset flag to be reused
							isFound = false;

							//Loop through timeArr in found date to see if time already exists
							for (int i = 0; i < jobArr[tempJobIndex].dateArr.arr[tempDateIndex].timeCount; i++)
							{
								if (jobArr[tempJobIndex].dateArr.arr[tempDateIndex].timeArr.arr[i].mTime == sTime)
								{
									isFound = true;
									tempTimeIndex = i;
								}
							}

							if (isFound) //sTime was found in dateArr's timeArr
							{
								dupLine = true;
								break;
							}
							else //sTime isn't in dateArr's timeArr
							{
								//Assign time and initialize elements
								newTime.mTime = sTime;
								newTime.lineNum = lineCount;

								//Enqueue time to dateArr's timeArr
								jobArr[tempJobIndex].dateArr.arr[tempDateIndex].timeArr.enqueue(newTime);

								//Increment count to reflect changes
								jobArr[tempJobIndex].dateArr.arr[tempDateIndex].timeCount++;

							}
						}
						else //sDate isn't in dateArr
						{
							//Assign time and initialize elements
							newTime.mTime = sTime;
							newTime.lineNum = lineCount;

							//Assign date and initialize elements
							newDate.mDate = sDate;
							newDate.timeCount = 0;

							//Enqueue time to newDate's timeArr
							newDate.timeArr.enqueue(newTime);

							//Increment count to reflect changes
							newDate.timeCount++;

							//If jobArr's dateArr is full
							if (jobArr[tempJobIndex].dateArr.isFull())
							{
								//Copy all time's line numbers from the front of the date queue into deleteLineArr
								for (int i = 0; i < jobArr[tempJobIndex].dateArr.arr[0].timeCount; i++)
								{
									deleteLineArr[numLinesToDelete] = jobArr[tempJobIndex].dateArr.arr[0].timeArr.arr[i].lineNum;
									numLinesToDelete++;
								}

								//Remove the first dateArr entry
								jobArr[tempJobIndex].dateArr.dequeue();

								//Assign and count the new date into JobArr
								jobArr[tempJobIndex].dateArr.enqueue(newDate);
								//Datecount remains the same

							}
							else
							{
								//Assign and count new date into JobArr
								jobArr[tempJobIndex].dateArr.enqueue(newDate);
								jobArr[tempJobIndex].dateCount++;
							}

							//Clean up temp variable
							newDate.timeArr.dequeue();
							newDate.timeCount = 0;
						}
					}
					else //jobName isn't in jobArr
					{
						jobArr[numUniqueJobs].name = sJobName;
						jobArr[numUniqueJobs].dateCount = 0;

						//Assign time and initialize elements
						newTime.mTime = sTime;
						newTime.lineNum = lineCount;

						//Assign date and initialize elements
						newDate.mDate = sDate;
						newDate.timeCount = 0;

						//Enqueue time to newDate's timeArr
						newDate.timeArr.enqueue(newTime);

						//Increment count to reflect changes
						newDate.timeCount++;

						//If jobArr's dateArr is full
						if (jobArr[numUniqueJobs].dateArr.isFull())
						{
							//Copy all time's line numbers from the front of the date queue into deleteLineArr
							for (int i = 0; i < jobArr[numUniqueJobs].dateArr.arr[0].timeCount; i++)
							{
								deleteLineArr[numLinesToDelete] = jobArr[numUniqueJobs].dateArr.arr[0].timeArr.arr[i].lineNum;
								numLinesToDelete++;
							}

							//Remove the first dateArr entry
							jobArr[numUniqueJobs].dateArr.dequeue();

							//Assign and count the new date into JobArr
							jobArr[numUniqueJobs].dateArr.enqueue(newDate);
							//Datecount remains the same

						}
						else
						{
							//Assign and count new date into JobArr
							jobArr[numUniqueJobs].dateArr.enqueue(newDate);
							jobArr[numUniqueJobs].dateCount++;
						}

						//Clean up temp variable
						newDate.timeArr.dequeue();
						newDate.timeCount = 0;

						//Increment count to reflect changes
						numUniqueJobs++;
					}
				}
				else
				{
					maxJobs = false;
					break;
				}
			}
		}//end while

		//Closes file
		din.close();
	}
	//If file did not open
	else
	{
		badFile = true;
	}

	//Error messages
	if (maxJobs)
	{
		cout << "Cannot add anymore jobs to the data set.\nIncrease MAX_RECORDS parameter before running again." << endl;
		cout << "This program will now exit." << endl;
	}
	if (badFile)
	{
		cout << "Error opening " << path << "\nCheck file name and permissions before running again." << endl;
		cout << "This program will now exit." << endl;
	}
	if (dupLine)
	{
		cout << "Duplicate line found at line number " << lineCount << "!\nOpen the file at " << path << " and make corrections.\nThis program will now exit. " << endl;
	}

	//If no errors, do post-work here
	if (!maxJobs && !badFile && !dupLine)
	{
		printDataToConsole();
		printDeleteLinesToConsole();
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printDataToConsole()
{
	for (int i = 0; i < numUniqueJobs; i++)
	{
		cout << jobArr[i].name << endl;
		for (int j = 0; j < jobArr[i].dateCount; j++)
		{
			cout << "\t" << jobArr[i].dateArr.arr[j].mDate << endl;
			for (int k = 0; k < jobArr[i].dateArr.arr[j].timeCount; k++)
			{
				cout << "\t\t" << jobArr[i].dateArr.arr[j].timeArr.arr[k].mTime << endl;
			}
		}
	}
}

void printDeleteLinesToConsole()
{
	for (int i = 0; i < numLinesToDelete; i++)
	{
		cout << "Need to delete line: " << deleteLineArr[i] << endl;
	}
}
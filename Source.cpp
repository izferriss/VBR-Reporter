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

/* YOU CAN CHANGE CONSTANTS AND PATH*/

//Constants
const int MAX_RECORDS = 1000;										//Number of unique jobs allowed

//Namespace (don't change)
using namespace std;

string path = "I:\\Temp\\PSTest-Ivan\\main_record.csv";				//use '\\' escape characters to infer '\' to compiler

/*DON'T CHANGE ANYTHING ELSE*/

//Variables
ifstream din(path);													//opens file for reading
string sLine;														//placeholder string for reading an entire line of the csv
size_t q1, q2, q3, q4, comma, dateSpace;							//position placeholders for string operations (q = quotation)
string sJobName, sDate, sTime;										//placeholder strings for reading in file elements
int numUniqueJobs = 0;												//keeps track of the number of elements in jobArr
int lineCount = 0;
int tempJobIndex, tempDateIndex, tempTimeIndex = -1;
bool isFound = false;

Job jobArr[MAX_RECORDS];
Date newDate;
TimeClass newTime;

int deleteLineArr[MAX_RECORDS];
int numLinesToDelete = 0;


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
			if (sizeof(jobArr) == 0)
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
						for(int i = 0; i < jobArr[tempJobIndex].dateArr.arr[tempDateIndex].timeCount; i++)
						{
							if (jobArr[tempJobIndex].dateArr.arr[tempDateIndex].timeArr.arr[i].mTime == sTime)
							{
								isFound = true;
								tempTimeIndex = i;
							}
						}

						if (isFound) //sTime was found in dateArr's timeArr
						{
							//error in logic or error in data? reee
							cout << "This job+date+time already exists..." << endl;
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
		for (int j = 0; j < jobArr[i].dateCount; j++)
		{
			cout << "\t" << jobArr[i].dateArr.arr[j].mDate << endl;
			for (int k = 0; k < jobArr[i].dateArr.arr[j].timeCount; k++)
			{
				cout << "\t\t" << jobArr[i].dateArr.arr[j].timeArr.arr[k].mTime << endl;
			}
		}
	}

	for (int i = 0; i < numLinesToDelete; i++)
	{
		cout << "Need to delete line: " << deleteLineArr[i] << endl;
	}
}
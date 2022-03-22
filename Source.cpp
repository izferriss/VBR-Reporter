#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Job.h"
#include "Date.h"
#include "TimeClass.h"
#include "TimeQueue.h"
#include "DateQueue.h"
#include "StringQueue.h"
#include "Constants.h"

using namespace std;

//Variables
ifstream din(PATH);													//opens file for reading
string sLine;														//placeholder string for reading an entire line of the csv
size_t q1, q2, q3, q4, comma, dateSpace, colonMIN,
       colonSEC, POSTSpace;											//placeholder indexes for string operations (q = quotation)
string sJobName, sDate, sTime, sTimeHR, sTimeMIN,
	   sTimeSEC, sTimePOST;											//placeholder strings for reading in file elements
int tempJobIndex, tempDateIndex, tempTimeIndex = -1;				//placeholder indexes used for adding new data to existing records

//Data Structures
Job jobArr[MAX_RECORDS];											//an array of Job classes - MAIN DATA STRUCTURE
Date newDate;														//a temporary Date class used to initialize variables before adding to jobArr - TEMP ASSIGNMENT
TimeClass newTime;													//a temporary TimeClass used to initalize variables before adding to Date class - TEMP ASSIGNMENT
int deleteLineArr[MAX_RECORDS];										//an array that keeps track of which lines to be deleted from the file we read from - HELPER
StringQueue dateHeaders;											//an array for keeping track of any of the dates that are in any job - HELPER

//Counters
int numUniqueJobs = 0;												//keeps track of the number of elements in jobArr											
int lineCount = 0;													//keeps track of the number of lines read in
int numLinesToDelete = 0;											//keeps track of the number of lines needed to be deleted

//Flags
bool isFound = false;												//used in searching for exists jobs/dates/times
bool badFile = false;												//used to display error message if there are issues opening the file
bool maxJobs = false;												//used to display error message if MAX_RECORDS is not big enough for the dataset
bool dupLine = false;												//used to display error message if there are duplicate lines in the file (shouldn't happen ever)
bool badJob = false;												//used to display error message if job name is blank

//Declarations
void printDataToConsole();
void printDeleteLinesToConsole();
void printDateHeadersToConsole();
void output();

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

			//Isolates date
			sDate = sLine.substr(q3 + 1, dateSpace - q3 - 1);
			if (sDate != "")
			{
				if (!dateHeaders.doesContain(sDate))
				{
					if (!dateHeaders.isFull())
					{
						dateHeaders.enqueue(sDate);
					}
					else
					{
						dateHeaders.dequeue();
						dateHeaders.enqueue(sDate);
					}
				}
			}
			//Isolates time
			sTime = sLine.substr(dateSpace + 1, q4 - dateSpace - 1);

			//Isolates hh:mm:ss tt
			colonMIN = sTime.find(":");
			colonSEC = sTime.find(":", colonMIN + 1);
			POSTSpace = sTime.find(" ", colonSEC + 1);
			sTimeHR = sTime.substr(0, colonMIN);
			sTimeMIN = sTime.substr(colonMIN + 1, colonSEC - colonMIN - 1);
			sTimeSEC = sTime.substr(colonSEC + 1, POSTSpace - colonSEC - 1);
			sTimePOST = sTime.substr(POSTSpace + 1, sTime.length() - 1);

			//convert hour to military
			if (sTimePOST == "PM" && sTimeHR != "12")
			{ 
				sTimeHR = to_string(stoi(sTimeHR) + 12);
			}
			else if (sTimePOST == "PM" && sTimeHR == "12")
			{
				sTimeHR = "12";
			}
			if (sTimePOST == "AM" && sTimeHR == "12")
			{
				sTimeHR = "00";
			}
			else if (sTimePOST == "AM" && sTimeHR != "10" && sTimeHR != "11")
			{
				sTimeHR = "0" + sTimeHR;
			}
			
			//set sTime to military instead of AM/PM
			sTime = sTimeHR + ":" + sTimeMIN + ":" + sTimeSEC;

			//jobArr is empty
			if (numUniqueJobs == 0 && numUniqueJobs < MAX_RECORDS && sJobName != "")
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
				if (numUniqueJobs < MAX_RECORDS && sJobName != "")
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
					if (sJobName == "")
					{
						badJob = true;
						break;
					}
					else
					{
						maxJobs = true;
						break;
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
		badFile = true;
	}

	//Error messages
	if (maxJobs)
	{
		cout << "ERROR: Cannot add anymore jobs to the data set.\nIncrease MAX_RECORDS parameter before running again." << endl;
		cout << "This program will now exit." << endl;
	}
	if (badJob)
	{
		cout << "WARNING: Cannot add job from line " << lineCount << " to data set.\nCheck input file for bad data before running again." << endl;
	}
	if (badFile)
	{
		cout << "ERROR: Error opening " << PATH << "\nCheck file name and permissions before running again." << endl;
		cout << "This program will now exit." << endl;
	}
	if (dupLine)
	{
		cout << "ERROR: Duplicate line found at line number " << lineCount << "!\nOpen the file at " << PATH << " and make corrections." << endl;
		cout << "This program will now exit. " << endl;
	}

	//If no errors, do post-work here
	if (!maxJobs && !badFile && !dupLine)
	{
		//printDataToConsole();
		//printDeleteLinesToConsole();
		//printDateHeadersToConsole();
		output();
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
			for (int k = jobArr[i].dateArr.arr[j].timeCount - 1; k > -1; k--)
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

void printDateHeadersToConsole()
{
	for (int i = 0; i < int(dateHeaders.rear +1); i++)
	{
		cout << dateHeaders.arr[i] << endl;
	}
}

void output()
{
	ofstream dout;
	bool match = false;
	int start = 0;
	string* copyArr = new string[MAX_TIMES];
	int copyArrLength = 0;

	cout << "Outputting HTML..." << endl;
	dout.open(OUTPUT);
	dout << "<HTML>\n";
	dout << "<HEAD>\n";
	dout << "\t<STYLE>\n";
	dout << "\n\t/* Body and Fonts */\n";				//HTML COMMENT
	dout << "\t*\n\t{\n\t\tfont-family: \"Lucida Console\", \"Courier New\", monospace;\n\t}\n";
	dout << "\n\t/* Center Alignment */\n";				//HTML COMMENT
	dout << "\t#report th, td\n\t{\n\t\tborder: 1px solid #cebdcd;\n\t\tposition: relative;\n\t\tempty-cells: show;\n\t\ttext-align: center;\n\t}\n";
	dout << "\n\t/* Job Name \"Headers\" */\n";			//HTML COMMENT
	dout << "\ttd.fauxheader\n\t{\n\t\tfont-weight: bold;\n\t}\n";
	dout << "\n\t/* Hover Effects */\n";				//HTML COMMENT
	dout << "\tth.rowheaders:hover\n\t{\n\t\tbackground-color: #bdcebe;\n\t\ttransition: 0.75s;\n\t\tcursor: pointer;\n\t\ttext-decoration: underline;\n\t}\n";
	dout << "\t#report td:hover::before\n\t{\n\t\tbackground-color: #e9efe9;\n\t\tcontent: '';\n\t\theight: 100%;\n\t\tleft: -5000px;\n\t\tposition: absolute;\n\t\ttop: 0;\n\t\twidth: 10000px;\n\t\tz-index: -2;\n\t}\n";
	dout << "\t#report td:hover::after\n\t{\n\t\tbackground-color: #e9efe9;\n\t\tcontent: '';\n\t\theight: 10000px;\n\t\tleft: 0;\n\t\tposition: absolute;\n\t\ttop: -5000px;\n\t\twidth: 100%;\n\t\tz-index: -1;\n\t}\n";
	dout << "\n\t/* Rounded Corners */\n";				//HTML COMMENT
	dout << "\tth:first-of-type\n\t{\n\t\tborder-top-left-radius: 10px;\n\t}\n";
	dout << "\tth:last-of-type\n\t{\n\t\tborder-top-right-radius: 10px;\n\t}\n";
	dout << "\ttr:last-of-type td:first-of-type\n\t{\n\t\tborder-bottom-left-radius: 10px;\n\t}\n";
	dout << "\ttr:last-of-type td:last-of-type\n\t{\n\t\tborder-bottom-right-radius: 10px;\n\t}\n";
	dout << "\n\t/* Table */\n";						//HTML COMMENT
	dout << "\t#report\n\t{\n\t\tbackground-clip: content-box;\n\t\tborder-collapse: separate;\n\t\tborder-spacing: 0pt;\n\t\tmargin-bottom: 80px;\n\t\tmargin-right: 80px;\n\t\toverflow: hidden;\n\t\tdisplay: inline-block;\n\t}\n";
	dout << "\tspan.note\n\t{\n\t\tfont-size: 75%;\n\t}\n";
	dout << "\t</STYLE>\n";
	dout << "</HEAD>\n";
	dout << "<BODY>\n";
	dout << "<TABLE id = \"report\">\n";
	dout << "<THEAD>\n";
	dout << "\t<TR>\n";
	dout << "\t\t<TH class =\"rowheaders\" onclick=\"sortTable(0)\">Job Name</TH>\n";
	for (int i = 0; i < dateHeaders.rear + 1; i++)
	{
		if (dateHeaders.arr[i] == "")
		{
			//This should never happen
			dout << "\t\t<TH>_____</TH>\n";
		}
		else
		{
			int j = i + 1;
			dout << "\t\t<TH class =\"rowheaders\" onclick=\"sortTable(" << j << ")\">" << dateHeaders.arr[i] << "</TH>\n";
		}
	}
	dout << "\t</TR>\n";
	dout << "</THEAD>\n";
	dout << "<TBODY>\n";

	//For each job
	for (int i = 0; i < numUniqueJobs; i++)
	{
		//Make a new row
		dout << "\t<TR>\n";
		//The first cell is the job name
		dout << "\t\t<TD class =\"fauxheader\">" << jobArr[i].name << "</TD>\n";
		//for each date in this job
		for (int j = 0; j < jobArr[i].dateArr.rear + 1; j++)
		{
			//for each date that's a header
			for (int k = start; k < dateHeaders.rear + 1; k++)
			{
				//if any header matches this job's date
				if (dateHeaders.doesContain(jobArr[i].dateArr.arr[j].mDate))
				{
					//if the date in this job is equal to the current header
					if (jobArr[i].dateArr.arr[j].mDate == dateHeaders.arr[k])
					{

						//If datearr's timecount is more than one
						if (jobArr[i].dateArr.arr[j].timeCount > 1)
						{
							//make a cell
							dout << "\t\t<TD>";

							//copy datearr's timearr to a new array
							for (int l = jobArr[i].dateArr.arr[j].timeArr.rear; l > -1; l--)
							{
								copyArr[l] = jobArr[i].dateArr.arr[j].timeArr.arr[l].mTime;
								//increment LCV
								copyArrLength++;
							}
							
							//sort ascending (hopefully)
							sort(copyArr, copyArr + copyArrLength);

							//output all times into cell
							for (int l = 0; l < copyArrLength; l++)
							{
								if (copyArr[l] != "")
								{
									dout << copyArr[l] << "<BR>";
								}
							}

							//close cell
							dout << "</TD>\n";

							//reset array memory and LCV
							delete[] copyArr;
							copyArr = new string[MAX_TIMES];
							copyArrLength = 0;
						}
						else
						{
							//make a cell
							dout << "\t\t<TD>";
							for (int l = jobArr[i].dateArr.arr[j].timeArr.rear; l > -1; l--)
							{
								dout << jobArr[i].dateArr.arr[j].timeArr.arr[l].mTime;
							}
							dout << "</TD>\n";
						}

						//mark that header complete
						start = k + 1;
						break;


					}
					//if the date in this job doesn't equal the current header
					else
					{
						//make a blank cell
						dout << "\t\t<TD></TD>\n";
					}
				}
			}
		}
		//if there are no more dates in a job that can be found in the headers, fill in the rest of the cells in that row with blanks
		while (start < dateHeaders.rear + 1)
		{
			start++;
			dout << "\t\t<TD></TD>\n";
		}
		//reset LCV for next row
		start = 0;

		//row end tag
		dout << "\t</TR>\n";

	}

	dout << "</TBODY>\n";
	dout << "</TABLE>\n";
	dout << "\n<BR>\n<BR>\n";
	dout << "<SPAN class = \"note\">Click on any header to sort the table by that column.</SPAN>\n<BR>\n";
	dout << "<SPAN class = \"note\">\t- This page will load with the last column sorted (ascending).</SPAN>\n<BR>\n";
	dout << "<SPAN class=\"note\">Any cell that contains multiple times will be sorted only by its first entry.</SPAN>\n<BR>\n";
	dout << "<SPAN class=\"note\">A blank cell is considered to have a time of 25:00:00.</SPAN>\n<BR>\n";
	dout << "</BODY>\n";
	dout << "<SCRIPT>\n";
	dout << "function sortTable(n)\n";
	dout << "\t{\n";
	dout << "\t\tvar table, rows, switching, i, x, y, shouldSwitch, dir, switchcount = 0;\n";
	dout << "\t\ttable = document.getElementById(\"report\");\n";
	dout << "\t\tswitching = true;\n";
	dout << "\t\tdir = \"asc\";\n";
	dout << "\t\twhile(switching)\n";
	dout << "\t\t\t{\n";
	dout << "\t\t\t\tswitching = false;\n";
	dout << "\t\t\t\trows = table.rows;\n";
	dout << "\t\t\t\tfor(i = 1; i <(rows.length - 1); i++)\n";
	dout << "\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\tshouldSwitch = false;\n";
	dout << "\t\t\t\t\t\t\t\tx = rows[i].getElementsByTagName(\"TD\")[n];\n";
	dout << "\t\t\t\t\t\t\t\ty = rows[i + 1].getElementsByTagName(\"TD\")[n];\n";
	dout << "\t\t\t\t\t\tif(dir == \"asc\")\n";
	dout << "\t\t\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\t\t\tif ((n == 0 && x.innerHTML.toLowerCase() > y.innerHTML.toLowerCase()) || (n != 0 && Number(convertDate(x.innerHTML)) > Number(convertDate(y.innerHTML))))\n";
	dout << "\t\t\t\t\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\t\t\t\t\tshouldSwitch = true;\n";
	dout << "\t\t\t\t\t\t\t\t\t\tbreak;\n";
	dout << "\t\t\t\t\t\t\t\t\t}\n";
	dout << "\t\t\t\t\t\t\t}\n";
	dout << "\t\t\t\t\t\telse if(dir == \"desc\")\n";
	dout << "\t\t\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\t\t\tif ((n == 0 && x.innerHTML.toLowerCase() < y.innerHTML.toLowerCase()) || (n != 0 && Number(convertDate(x.innerHTML)) < Number(convertDate(y.innerHTML))))\n";
	dout << "\t\t\t\t\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\t\t\t\t\tshouldSwitch = true;\n";
	dout << "\t\t\t\t\t\t\t\t\t\tbreak;\n";
	dout << "\t\t\t\t\t\t\t\t\t}\n";
	dout << "\t\t\t\t\t\t\t}\n";
	dout << "\t\t\t\t\t}\n";
	dout << "\t\t\t\tif (shouldSwitch)\n";
	dout << "\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\trows[i].parentNode.insertBefore(rows[i + 1], rows[i]);\n";
	dout << "\t\t\t\t\t\tswitching = true;\n";
	dout << "\t\t\t\t\t\tswitchcount++;\n";
	dout << "\t\t\t\t\t}\n";
	dout << "\t\t\t\telse\n";
	dout << "\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\tif(switchcount == 0 && dir == \"asc\")\n";
	dout << "\t\t\t\t\t\t\t{\n";
	dout << "\t\t\t\t\t\t\t\tdir = \"desc\";\n";
	dout << "\t\t\t\t\t\t\t\tswitching = true;\n";
	dout << "\t\t\t\t\t\t\t}\n";
	dout << "\t\t\t\t\t}\n";
	dout << "\t\t\t}\n";
	dout << "\t}\n";
	dout << "function convertDate(inDate)\n";
	dout << "\t{\n";
	dout << "\t\tif (inDate == \"\")\n";
	dout << "\t\t\t{\n";
	dout << "\t\t\t\treturn (250000);\n";
	dout << "\t\t\t}\n";
	dout << "\t\telse\n";
	dout << "\t\t\t{\n";
	dout << "\t\t\t\tvar part = inDate.split(\":\");\n";
	dout << "\t\t\t\treturn (part[0]+part[1]+part[2].substr(0, 2));\n";
	dout << "\t\t\t}\n";
	dout << "\t}\n";
	dout << "sortTable(" << dateHeaders.rear + 1 << ")";
	dout << "</SCRIPT>\n";
	dout << "</HTML>";
	dout.close();
}
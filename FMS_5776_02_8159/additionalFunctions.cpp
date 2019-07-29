#pragma once
/*******************************************************************\
File: additionalFunctions.cpp ;    Path: FMS_5776_02_8159\additionalFunctions.cpp
Description: Implemnts globalFunctions.h
\*******************************************************************/
#include "additionalFunctions.h"

string getCurrentDate()
{
	time_t curTime = time(NULL);
	tm* ltm = new tm();
	localtime_s(ltm, &curTime);
	char charDate[26];	// size of asctime string
	asctime_s(charDate, ltm);
	string temp(charDate);

	int day, month;
	string dd_str, mm_str, yyyy_str;
	char dd[2 + 1], mm[2 + 1];	// +1 for \0
	yyyy_str = temp.substr(temp.size() - 5, 4);
	day = ltm->tm_mday;
	month = ltm->tm_mon + 1;	//	+1 because the range of months in tm struct is 0-11
	_itoa_s(day, dd, 10);
	_itoa_s(month, mm, 10);
	dd_str.assign(dd);
	mm_str.assign(mm);

	if (day < 10)
		dd_str.insert(0, "0");
	if (month < 10)
		mm_str.insert(0, "0");

	string targetFormat = dd_str + '/' + mm_str + '/' + yyyy_str;

	delete ltm;
	return targetFormat;
}
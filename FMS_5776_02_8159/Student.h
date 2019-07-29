#pragma once
#include "Disk.h"
using namespace std;

struct Student
{
	int ID;
	char Name[10];
	int grade;
	char _emptyArea[480];

	Student()
	{}

	Student(char a)
	{
		grade = 0;
		ID = 0;
		strcpy_s(Name, "0\0");
	}


	void PrintInfo()
	{
		cout << endl << "Name: " << Name << " ID: " << ID << " grade: " << grade << endl;
	}

	void CreateNewStudent()
	{
		cout << "Creating a new Student!" << endl;

		cout << "Enter Student's Name Please:" << endl;
		cin >> Name;
		cout << "Enter Student's ID Please:" << endl;
		cin >> ID;
		cout << "Enter Student's grade Please:" << endl;
		cin >> grade;
		cout << "Done!" << endl;



	}
	friend ostream&  operator << (Student a, ostream & B)
	{
		B << a.Name << " " << a.ID << " " << a.grade << endl;
		return B;
	}
};
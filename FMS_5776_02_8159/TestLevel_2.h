#pragma once
#include "Disk.h"
using namespace std;

class TestLevel_2
{
	static void PrintDAT(Disk& D)
	{
		for (int i = 0; i < 1600; i++)
			cout << D._dat._DAT[i] << ' ';

		cout << "Number of Clusters That Are 'Free' -->" << D.howMuchEmpty() << endl;
		
	}

public:
	static void test_2()
	{
		try
		{
			string DiskName = "Disk 1";
			string OwnerName = "Hugh";
			string FileType, KeyType;
			int Choice = 1, MiniChoice = 3;
			int FitChoice = 0;
			Disk D(DiskName, OwnerName, 'c');
			D.format(OwnerName);
			int MaxRecLnth, NumOfSectrs, KeyOffset, KeyLength = 20;			// keylength is optional...

			while (Choice)
			{
				cout << endl;
				cout << "--------------------------------------------------" << endl;
				cout << " Please Select An Option From The Following : " << endl;
				cout << " 1 - Create A New File" << endl;
				cout << " 2 - Extend An Existing File" << endl;
				cout << " 3 - Delete A File" << endl;
				cout << " 4 - Print DAT" << endl;
				cout << " 0 - Exit" << endl;
				cout << "--------------------------------------------------" << endl;

				cin >> Choice;

				cout << endl;
				switch (Choice)
				{
				case 0:
					return;
				case 1:															// Create A New File
					cout << "Enter The New File Name Please" << endl;
					cin >> DiskName;
					cout << "Please Enter The New File Owner's Name" << endl;
					cin >> OwnerName;
					while (MiniChoice != 1 && MiniChoice != 2)
					{
						cout << "Select The Relevant File Type Please:" << endl;
						cout << " 1 - Fixed Length Records" << endl;
						cout << " 2 - Variable Length Records" << endl;

						cin >> MiniChoice;
						switch (MiniChoice)
						{
						case 1:
							FileType = 'F';
							break;
						case 2:
							FileType = 'V';
							break;
						default:
							break;
						}
					}
					MiniChoice = 6;
					cout << "Please Enter The Maximum Record Length" << endl;
					cin >> MaxRecLnth;
					cout << "Please Enter The Size Of The New File In Sectors (Each Sector Take 1024 Bytes)" << endl;
					cin >> NumOfSectrs;
					while (MiniChoice < 1 || MiniChoice > 4)
					{
						cout << "Select The Relevant Key Type Please:" << endl;
						cout << " 1 - Integer Type (int)" << endl;
						cout << " 2 - Float Type" << endl;
						cout << " 3 - Double Type" << endl;
						cout << " 4 - String Or Character Type" << endl;
						cin >> MiniChoice;
						switch (MiniChoice)
						{
						case 1:
							KeyType = 'I';
							break;
						case 2:
							KeyType = 'F';
							break;
						case 3:
							KeyType = 'D';
							break;
						case 4:
							KeyType = 'C';
							cout << "Please Enter The Key Length" << endl;
							cin >> KeyLength;
							break;
						default:
							break;
						}
					}
					cout << "Please Enter The Key's Offset In The Records" << endl;
					cin >> KeyOffset;
					cout << "Which Allocation algorithm Would You Like To Use?" << endl;
					cout << "	0 - First Fit Algorithm" << endl;
					cout << "	1 - Best Fit Algorithm" << endl;
					cout << "	2 - Worst Fit Algorithm" << endl;
					cin >> FitChoice;

					D.createFile(DiskName, OwnerName, MaxRecLnth, NumOfSectrs, KeyOffset, FitChoice);
					cout << "Done!" << endl;
					break;
				case 2:															// Extend An Existing File
					cout << "Enter The File Name Please" << endl;
					cin >> DiskName;
					cout << "Enter The File Owner's Name Please" << endl;
					cin >> OwnerName;
					cout << "Please Enter The Number Of Sectors You Would Like To Add To The File" << endl << "(Each Sectors Conatins 1024 Byets)" << endl;
					cin >> NumOfSectrs;
					cout << "Which Allocation algorithm Would You Like To Use?" << endl;
					cout << "	0 - First Fit Algorithm" << endl;
					cout << "	1 - Best Fit Algorithm" << endl;
					cout << "	2 - Worst Fit Algorithm" << endl;
					cin >> FitChoice;
					D.extendFile(DiskName, OwnerName, NumOfSectrs, FitChoice);
					break;
				case 3:															// Delete A File
					cout << "Enter The File Name Please" << endl;
					cin >> DiskName;
					cout << "Please Enter The File Owner's Name" << endl;
					cin >> OwnerName;
					D.delFile(DiskName, OwnerName);
					break;
				case 4:															// Print DAT
					PrintDAT(D);
					break;
				default:
					break;
				}
			}
		}
		catch (char * qwerty)
		{
			cout << qwerty << endl;
		}
		catch (exception qwerty)
		{
			cout << qwerty.what() << endl;
		}
		catch (...)
		{
			cout << "something is wrong" << endl;
		}

	}
};
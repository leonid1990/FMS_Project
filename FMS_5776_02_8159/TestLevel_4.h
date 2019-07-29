#pragma once
#include "Disk.h"
#include "FCB.h"
#include "DMS.h"
#include "Student.h"
using namespace std;

class TestLevel_4
{
	static void PrintDAT(Disk& D)
	{
		for (int i = 0; i < 1600; i++)
			cout << D._dat._DAT[i] << ' ';

		cout << "Number of Clusters That Are 'Free' -->" << D.howMuchEmpty() << endl;
	}
	static int findFCB(DMS& dms)
	{
		int Index = -1, choice = 1;
		cout << "Please Select A File From The Following:" << endl;
		for (int i = 0; i < dms._fcbArrSize; i++)
		{
			if (dms._fcbArray[i]._d != NULL)
			{
				cout << choice << " - " << dms._fcbArray[i]._fileDesc._fileName << endl;
				choice++;
			}
		}
		cout << endl;
		cin >> Index;
		choice = 1;
		for (int i = 0; i < dms._fcbArrSize; i++)
		{
			if (choice == Index)
			{
				Index = i;
				break;
			}
			if (dms._fcbArray[i]._d != NULL)
				choice++;
		}

		return Index;
	}
public:


	static void test_4()
	{
		cout << "1 - A Student => sizeof(Student) = " << sizeof(Student) << endl;
		try
		{
			string DiskName = "Disk 1";
			string OwnerName = "Hugh";
			bool Lsb = false, fileOpened;
			int choice = 4, MiniChoice = 3, fcbIndex;
			char a;
			string FileType, KeyType, temp;;
			int MaxRecLnth, NumOfSectrs, KeyOffset, Index, KeyLength = 20, allocchoice;
			FCB * aFile = NULL;
			Student stud2('a');
			DMS dms;
			DirEntry* entryPtr;

			while (choice > 2 || choice < 1)
			{
				cout << endl << "Please select one option:" << endl;
				cout << "1 - Create a new Disk" << endl;
				cout << "2 - Mount a Disk" << endl;
				cin >> choice;
				switch (choice)
				{
				case 1:
					a = 'c';
					break;
				case 2:
					a = 'm';
					break;
				default:
					break;
				}
			}

			Disk D(DiskName, OwnerName, a);
			
			while (choice)
			{
				cout << endl;
				cout << "--------------------------------------------------" << endl;
				cout << " Please Select An Option From The Following : " << endl;
				cout << " 0 - Exit" << endl;
				cout << " 1 - Print DAT" << endl;
				cout << " 2 - Format the disk" << endl;
				cout << " 3 - Create a new file" << endl;
				cout << " 4 - Open a file" << endl;
				cout << " 5 - Close a file" << endl;
				cout << " 6 - Delete a file" << endl;
				cout << " 7 - Add a student" << endl;
				cout << " 8 - Seek a student" << endl;
				cout << " 9 - Read a specific student's info" << endl;
				cout << " 10 - Update a specific student's info" << endl;
				cout << " 11 - Delete a student" << endl;
				cout << " 12 - printing the whole content of the file" << endl;
				cout << " 13 - Unmount the disk" << endl;

				cout << "--------------------------------------------------" << endl;

				cin >> choice;

				cout << endl;
				switch (choice)
				{
				case 0:															//	Exit;
					break;
				case 1:															// Print DAT
					PrintDAT(D);
					break;
				case 2:
					D.format(OwnerName);
					break;
				case 3:															// Create a new file 
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
					cout << "Please Enter The Actual Record Length" << endl;
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
					cout << "Please Enter The allocation choice" << endl;
					cin >> allocchoice;

					D.createFile(DiskName, OwnerName, MaxRecLnth, NumOfSectrs, KeyOffset, allocchoice);
					cout << "Done..!" << endl;
					break;
				case 4:															// Open a file
					cout << "Please Select A File From The Following:" << endl;
					NumOfSectrs = 1;											// NumOfSectrs - is used as a regular variable in this case..

					for (int i = 0; i < 17; i++)
					{
						if (D._rootDir._lsbSector._dirEntry[i]._entryStatus == '1')
						{
							cout << NumOfSectrs << " - " << D._rootDir._lsbSector._dirEntry[i]._fileName<<endl;
							NumOfSectrs++;
						}
					}
					for (int i = 0; i < 17; i++)
					{
						if (D._rootDir._msbSector._dirEntry[i]._entryStatus == '1')
						{
							cout << NumOfSectrs << " - " << D._rootDir._msbSector._dirEntry[i]._fileName<<" ";
							NumOfSectrs++;
						}
					}

					cout << endl;
					cin >> Index;
					NumOfSectrs = 1;
					for (int i = 0; i < 17; i++)
					{
						if (NumOfSectrs == Index)
						{
							Index = i;
							Lsb = true;
							break;
						}
						if (D._rootDir._lsbSector._dirEntry[i]._entryStatus == '1')
							NumOfSectrs++;
					}

					for (int i = 0; i < 17; i++)
					{
						if (NumOfSectrs == Index)
						{
							Index = i;
							Lsb = false;
							break;
						}
						if (D._rootDir._msbSector._dirEntry[i]._entryStatus == '1')
							NumOfSectrs++;
					}
					cout << "Please enter your User Name:" << endl;
					cin >> OwnerName;

					choice = 5;
					while (choice > 4 || choice < 1)
					{
						cout << "Select the desired Read-Write mode" << endl;
						cout << "1 - Read-Write - 'I/O'" << endl;
						cout << "2 - Read only - 'I'" << endl;
						cout << "3 - Write only - 'O'" << endl;
						cout << "4 - Extend-File mode - 'E'" << endl;
						cin >> choice;
						switch (choice)
						{
						case 1:
							FileType = "IO";
							break;
						case 2:
							FileType = "I";
							break;
						case 3:
							FileType = "O";
							break;
						case 4:
							FileType = "E";
							break;
						default:
							break;
						}
					}


					if (Lsb)
						aFile = dms.openFile(&D, (string)D._rootDir._lsbSector._dirEntry[Index]._fileName, OwnerName, FileType);
					else aFile = dms.openFile(&D, (string)D._rootDir._msbSector._dirEntry[Index]._fileName, OwnerName, FileType);

					if (aFile)
						cout << "The File was successfully opened!";
					break;
				case 5:															// Close a file
					fcbIndex = findFCB(dms);
					if (fcbIndex == -1)
						throw exception("Error: No files open.");
					aFile = &dms._fcbArray[fcbIndex];
					aFile->closeFile();
					cout << "Done!" << endl;
					break;
				case 6:
					fileOpened = false;
					cout << "Please Select A File From The Following:" << endl;
					NumOfSectrs = 1;											// NumOfSectrs - is used as a regular variable in this case..

					for (int i = 0; i < 17; i++)
					{
						if (D._rootDir._lsbSector._dirEntry[i]._entryStatus == '1')
						{
							cout << NumOfSectrs << " - " << D._rootDir._lsbSector._dirEntry[i]._fileName<<endl;
							NumOfSectrs++;
						}
					}
					for (int i = 0; i < 17; i++)
					{
						if (D._rootDir._msbSector._dirEntry[i]._entryStatus == '1')
						{
							cout << NumOfSectrs << " - " << D._rootDir._msbSector._dirEntry[i]._fileName<<endl;
							NumOfSectrs++;
						}
					}

					cout << endl;
					cin >> Index;
					NumOfSectrs = 1;
					for (int i = 0; i < 17; i++)
					{
						if (NumOfSectrs == Index)
						{
							Index = i;
							Lsb = true;
							break;
						}
						if (D._rootDir._lsbSector._dirEntry[i]._entryStatus == '1')
							NumOfSectrs++;
					}

					for (int i = 0; i < 17; i++)
					{
						if (NumOfSectrs == Index)
						{
							Index = i;
							Lsb = false;
							break;
						}
						if (D._rootDir._msbSector._dirEntry[i]._entryStatus == '1')
							NumOfSectrs++;
					}
					cout << "Please enter your User Name:" << endl;
					cin >> OwnerName;
					if (Lsb)
						entryPtr = &D._rootDir._lsbSector._dirEntry[Index];
					else
						entryPtr = &D._rootDir._msbSector._dirEntry[Index];
					for (int i = 0; i < dms._fcbArrSize; i++)
					{
						if (dms._fcbArray[i]._d == NULL)
							break;
						if (Lsb)
							temp = dms._fcbArray[i]._d->_rootDir._lsbSector._dirEntry->_fileName;
						else
							temp = dms._fcbArray[i]._d->_rootDir._msbSector._dirEntry->_fileName;
						if (temp == entryPtr->_fileName)
						{
							fileOpened = true;
							break;
						}
					}
					if (fileOpened)
						throw exception("Error: Please close the file first.");
					D.delFile(string(entryPtr->_fileName), OwnerName);
					cout << "The File was successfully deleted!";
					break;
				case 7:															// Add a student
					aFile = &dms._fcbArray[findFCB(dms)];
					stud2.CreateNewStudent();
					aFile->writeRec((char*)&stud2);
					aFile->_fileDesc._eofRecNr++;
					break;
				case 8:															// Seek a student
					aFile = &dms._fcbArray[findFCB(dms)];
					cout << "Enter how much student's records you would like to jump please:" << endl;
					cin >> KeyLength;

					MiniChoice = 4;
					while (MiniChoice < 0 || MiniChoice > 3)
					{
						cout << "Select from where you would like to start jumping:" << endl;
						cout << "1 - from the beginning of the file." << endl;
						cout << "2 - from the current location." << endl;
						cout << "3 - from the end of the file." << endl;

						cin >> MiniChoice;

						switch (MiniChoice)
						{
						case 1:											// from the beginning of the file
							aFile->seekRec(0, KeyLength);
							break;
						case 2:											// from the current location
							aFile->seekRec(1, KeyLength);
							break;
						case 3:											// from the end of the file
							aFile->seekRec(2, KeyLength);
							break;
						default:
							break;
						}
					}

					cout << "The requested student's info:" << endl;

					aFile->readRec((char*)&stud2, 0);
					stud2.PrintInfo();

					cout << endl << "Done!" << endl;

					break;
				case 9:															// Read a specific student's info
					aFile = &dms._fcbArray[findFCB(dms)];
					cout << "Enter the ID of the wanted Student please:" << endl;
					cin >> KeyLength;


					aFile->seekRec(0, 0);
					for (int J = 0; J < aFile->_fileDesc._eofRecNr;)
					{
						aFile->readRec((char*)&stud2, 0);

						if (stud2.ID == KeyLength)
							break;

						if (stud2.ID != NULL)
							J++;
					}

					if (stud2.ID != KeyLength)
						throw exception("The requsted student is not exist!");
					else  stud2.PrintInfo();
					cout << "Done!" << endl;
					break;
				case 10:															// Update a specific student's info
					aFile = &dms._fcbArray[findFCB(dms)];
					cout << "Enter the ID of the wanted Student please:" << endl;
					cin >> KeyLength;

					aFile->seekRec(0, 0);
					for (int i = 0; i < aFile->_fileDesc._eofRecNr; i++)
					{
						aFile->readRec((char*)&stud2, 0);

						if (stud2.ID == KeyLength)
							break;
					}
					aFile->seekRec(1, -1);
					aFile->readRec((char*)&stud2, 1);
					if (stud2.ID != KeyLength)
						throw exception("The requsted student is not exist!");
					else  stud2.PrintInfo();

					cout << "Enter the updated information please:" << endl;
					cout << endl << "Name: ";
					cin >> stud2.Name;
					cout << "ID: ";
					cin >> stud2.ID;
					cout << "grade: ";
					cin >> stud2.grade;
					cout << endl;

					cout << "Would you like to cancel the update?	Enter Y/N" << endl;
					cin >> a;
					if (a == 'y' || a == 'Y')
					{
						aFile->updateCancel();
					}
					else aFile->updateRec((char*)&stud2);
					cout << "Done!" << endl;

					break;
				case 11:															// Delete a student
					aFile = &dms._fcbArray[findFCB(dms)];
					cout << "Enter the ID of the wanted Student please:" << endl;
					cin >> KeyLength;

					aFile->seekRec(0, 0);
					for (int i = 0; i < aFile->_fileDesc._eofRecNr; i++)
					{
						aFile->readRec((char*)&stud2, 0);

						if (stud2.ID == KeyLength)
							break;
					}
					if (stud2.ID != KeyLength)
						throw exception("The requsted student is not exist!");

					cout << "You are about to delete:" << endl;
					stud2.PrintInfo();
					cout << "Are you sure ?? Y/N" << endl;
					cin >> a;
					if (a == 'y' || a == 'Y')
					{
						aFile->seekRec(1, -1);
						aFile->readRec((char*)&stud2, 1);
						aFile->deleteRec();
					}

					cout << "Done!" << endl;

					break;
				case 12:														// printing the whole content of the file
					aFile = &dms._fcbArray[findFCB(dms)];
					aFile->seekRec(0, 0);
					for (int J = 0; J < aFile->_fileDesc._eofRecNr;)
					{
						aFile->readRec((char*)&stud2, 0);
						if (stud2.ID != NULL)
						{
							stud2.PrintInfo();
							J++;
						}
					}

					break;
				case 13:
					dms.unmount(&D);
					if (D._mounted == false)
						cout << "The disk was successfully unmounted!";
					break;
				default:
					break;
				}
			}
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
	}
};
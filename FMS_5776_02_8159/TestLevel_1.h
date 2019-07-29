#pragma once
#include "Disk.h"
#include <list>
using namespace std;

struct FATStruct
{
public:
	int FATStcNumber;
	DATtype FAT;
	int NumOfAllocatedSectors;

	FATStruct()	{}
	bool operator == (FATStruct b)
	{
		if (FATStcNumber == b.FATStcNumber)
			return true;
		else return false;
	}

};


class TestLevel_1
{
	static void PrintDAT(Disk& D)
	{
		for (int i = 0; i < 1600; i++)
			cout << D._dat._DAT[i] << ' ';

		cout << "Number of Clusters That Are 'Free' -->" << D.howMuchEmpty() << endl;

	}
	static void DeallocateAFAT(Disk &D, list<FATStruct> &Flist)
	{
		bool Happend = false;
		int choice, k;
		cout << "Please Select A FAT TO Deallocate From The Following:" << endl;
		list<FATStruct> ::iterator it = Flist.begin();
		for (int i = 1; it != Flist.end(); it++, i++)
		{
			Happend = true;
			cout << i << " - " << it->NumOfAllocatedSectors << endl;

		}

		if (!Happend)
		{
			cout << endl << "There Are No FAT'S Here." << endl << endl;
			return;
		}


		cin >> choice;

		if (choice < 1 || choice >(int)Flist.size())
			throw exception("There Is No Such Option!");

		it = Flist.begin();
		for (k = 1; k < choice; k++, it++)		{}

		D.dealloc(it->FAT);
		Flist.remove(*it);
		cout << "Done..!!" << endl << endl;
	}
	static void AllocationCheck(Disk &D, FATStruct *FATStructure, list<FATStruct> &Flist)
	{
		int NumOfSecToAllc, MiniChoice = 1;
		FATStructure->FAT.reset();
		while (MiniChoice)
		{
			cout << "Which Algorithm Would You Like To Test ??" << endl;
			cout << "	1 - First Fit Algorithm" << endl;
			cout << "	2 - Best Fit Algorithm" << endl;
			cout << "	3 - Worst Fit Algorithm" << endl;
			cout << "	4 - Back To The Last Menu" << endl << endl;

			cin >> MiniChoice;

			if (MiniChoice > 4 || MiniChoice < 1)
			{
				cout << "Make Sure You Are Selecting A Valid Option !!" << endl << endl << endl;
				continue;
			}

			if (MiniChoice == 4)
				return;
			cout << "Please Enter Number Of Sectors To Allocate:" << endl;
			cin >> NumOfSecToAllc;
			if (NumOfSecToAllc > D.howMuchEmpty() * 2)
			{
				cout << "The FAT Size That You've Selected Is Too Big..!!" << endl << endl << endl;
				continue;
			}
			FATStructure->NumOfAllocatedSectors = NumOfSecToAllc;

			switch (MiniChoice)
			{

			case 1:																// First Fit Algorithm
				D.alloc(FATStructure->FAT, NumOfSecToAllc, 0);
				Flist.insert(Flist.end(), *FATStructure);
				cout << "Done!!" << endl;
				return;
			case 2:																// Best Fit Algorithm
				D.alloc(FATStructure->FAT, NumOfSecToAllc, 1);
				Flist.insert(Flist.end(), *FATStructure);
				cout << "Done!!" << endl;
				return;
			case 3:																// Worst Fit Algorithm
				D.alloc(FATStructure->FAT, NumOfSecToAllc, 2);
				Flist.insert(Flist.end(), *FATStructure);
				cout << "Done!!" << endl;
				return;
			default:
				cout << "Make Sure You Are Selecting A Valid Option" << endl;
				return;
			}

		}

	}
	static void ExtededAllocationCheck(Disk &D, list<FATStruct> &Flist)
	{
		bool Happend = false;
		int choice, k;
		cout << "Please Select A FAT To Extend From The Following:" << endl;
		list<FATStruct> ::iterator it = Flist.begin();

		for (int i = 1; it != Flist.end(); it++, i++)
		{
			Happend = true;
			cout << i << " - " << it->NumOfAllocatedSectors << endl;
		}

		if (!Happend)
		{
			cout << endl << "There Are No FAT'S Here.." << endl << endl;
			return;
		}

		cin >> choice;

		if (choice < 1 || choice >(int)Flist.size())
			cout << "There Is No Such Option..!" << endl;

		it = Flist.begin();
		for (k = 1; k < choice; k++, it++)		{}

		int NumOfSecToAdd, MiniChoice = 1;

		while (MiniChoice)
		{
			cout << "Which Algorithm Would You Like To Use ??" << endl;
			cout << "	1 - First Fit Algorithm" << endl;
			cout << "	2 - Best Fit Algorithm" << endl;
			cout << "	3 - Worst Fit Algorithm" << endl;
			cout << "	4 - Back To The Last Menu" << endl << endl;

			cin >> MiniChoice;

			if (MiniChoice > 4 || MiniChoice < 1)
			{
				cout << "Make Sure You Are Selecting A Valid Option !!" << endl << endl << endl;
				continue;
			}

			if (MiniChoice == 4)
				return;
			cout << "Please Enter The Number Of Sectors You Would Like To Add:" << endl;
			cin >> NumOfSecToAdd;
			if (NumOfSecToAdd > D.howMuchEmpty() * 2)
			{
				cout << "The FAT Size That You've Selected Is To Big..!!" << endl << endl << endl;
				continue;
			}

			it->NumOfAllocatedSectors += NumOfSecToAdd;

			switch (MiniChoice)
			{

			case 1:																// First Fit Algorithm
				D.allocextend(it->FAT, NumOfSecToAdd, 0);
				cout << "Done!!" << endl;
				return;
			case 2:																// Best Fit Algorithm
				D.allocextend(it->FAT, NumOfSecToAdd, 1);
				cout << "Done!!" << endl;
				return;
			case 3:																// Worst Fit Algorithm
				D.allocextend(it->FAT, NumOfSecToAdd, 2);
				cout << "Done!!" << endl;
				return;
			default:
				cout << "Make Sure You Are Selecting A Valid Option" << endl;
				return;
			}

		}
	}

public:
	static void test_1()
	{

		int FATStrucCounter = 0;
		try
		{
			string DiskName = "Disk 1";
			string OwnerName = "Hugh";
			char a;
			int choice = 1;
			Disk D(DiskName, OwnerName, 'c');
			D.format(OwnerName);

			FATStruct *FATstrc = new FATStruct;
			list<FATStruct> FATStrcList;

			while (choice)
			{
				cout << endl;
				cout << "--------------------------------------------------" << endl;
				cout << " Please Select An Option From The Following : " << endl;
				cout << " 1 - Print DAT" << endl;
				cout << " 2 - Checking The Algorithms of Allocation " << endl;
				cout << " 3 - Checking The Algorithms of An Extended Allocation " << endl;
				cout << " 4 - Deallocate A FAT" << endl;
				cout << " 5 - Format Disk" << endl;
				cout << " 0 - Exit" << endl;
				cout << "--------------------------------------------------" << endl;

				cin >> choice;
				cout << endl;
				switch (choice)
				{
				case 0:
					//	return;
					break;
				case 1:															// Print DAT
					PrintDAT(D);
					break;
				case 2:															// Checking The Algorithms of Allocation 
					FATstrc->FATStcNumber = ++FATStrucCounter;
					AllocationCheck(D, FATstrc, FATStrcList);
					break;
				case 3:															// Checking The Algorithms of An Extended Allocation
					FATstrc->FATStcNumber = ++FATStrucCounter;
					ExtededAllocationCheck(D, FATStrcList);
					break;
				case 4:															// Deallocate A FAT
					DeallocateAFAT(D, FATStrcList);
					break;
				case 5:
					cout << "Please Enter The Name Of The Disk Owner Inorder To Format Disk :" << endl;
					cin >> OwnerName;
					cout << "This Operation Will Format The Disk Are You Sure..??!" << endl << "Enter Y/N" << endl;
					cin >> a;
					if (a != 'y' && a != 'Y')
						break;
					D.format(OwnerName);
					FATstrc->FATStcNumber = 0;
					FATStrcList.clear();

					FATStrucCounter = 0;
					break;
				default:
					break;
				}
			}
			D.writeSector(1, (Sector*)&D._dat);
			D._dat._sectorNr = 3199;
			D.writeSector(3199, (Sector*)&D._dat);
			D._dat._sectorNr = 1;
			D._dat._DAT.reset();
			D.readSector(1, (Sector*)&D._dat);
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
	}
};
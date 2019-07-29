#pragma once
/*******************************************************************\
File: Disk.cpp ;	Path: FMS_5776_02_8159\Disk.cpp
\*******************************************************************/

#include "Disk.h"
#include "FCB.h"
#include <time.h>
using namespace std;

/*-------------------------------------------------------------------*/
							//	Level 0
Disk::Disk(){}

Disk::Disk(string& dskflName, string& ownerName, char code)
{
	if (code == 'c')
	{
		try
		{
			createdisk(dskflName, ownerName);
			mountdisk(dskflName);
		}
		catch (exception exc)
		{
			cout << exc.what() << endl;
			throw exc;
		}
	}
	else if (code == 'm')
	{
		try
		{
			mountdisk(dskflName);
		}
		catch (exception exc)
		{
			cout << exc.what() << endl;
			throw exc;
		}
	}
	else
		throw exception("Error: Wrong code. Please enter 'c' or 'm'");
}

Disk::~Disk()
{
	if (_mounted == true)
		try
		{
			unmountdisk();
		}
		catch (exception exc)
		{
			cout << exc.what() << endl;
			throw exc;
		}
}

void Disk::createdisk(string& diskName, string& ownerName)
{
	_dskfl.open(diskName, ios::out | ios::binary | ios::trunc);
	if (!_dskfl)
		throw exception("Error: Creating file failed. Disk won't be created.");
	Sector sector;
	for (unsigned int i = 0; i < 3200; i++)
	{
		sector._sectorNr = i;
		try
		{
			writeSector(i, &sector);
		}
		catch (exception exc)
		{
			cout << exc.what() << endl;
			throw exc;
		}
	}
	try
	{
		initialize_vhd(diskName, ownerName);
		writeSector(0, (Sector*)&_vhd);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
	_dskfl.close();
	_vhd._isFormated = false;
}

void Disk::mountdisk(string& dskflName)
{
	if (_mounted)
		throw exception("Error: File doesn't exist. Disk won't be mounted.");
	_dskfl.open(dskflName, ios::in | ios::out | ios::binary);
	if (!_dskfl)
		throw exception("Error: File doesn't exist. Disk won't be mounted.");
	try
	{
		readSector(0, (Sector*)&_vhd);
		readSector(1, (Sector*)&_dat);
		readSector(2, (Sector*)&_rootDir._msbSector);
		readSector(3, (Sector*)&_rootDir._lsbSector);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
	_mounted = true;
}

void Disk::unmountdisk()
{
	if (_mounted == false)
		throw exception("Error: There is no mounted disk.");
	try
	{
		writeSector(0, (Sector*)&_vhd);
		writeSector(1, (Sector*)&_dat);
		writeSector(2, (Sector*)&_rootDir._msbSector);
		writeSector(3, (Sector*)&_rootDir._lsbSector);
	// write the backup for DAT and rootDir
		writeSector(_vhd._addrDATcpy, (Sector*)&_dat);
		writeSector(_vhd._addrRootDirCpy * 2, (Sector*)&_rootDir._msbSector);
		writeSector(_vhd._addrRootDirCpy * 2 + 1, (Sector*)&_rootDir._lsbSector);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
	_dskfl.close();
	_mounted = false;
}

void Disk::recreatedisk(string& diskName, string& ownerName)
{
	if (_mounted)
		throw exception("Error: Disk is mounted.");
	if (strcmp(ownerName.c_str(), _vhd._diskOwner) != 0)
		throw "Error: The owner is not " + ownerName + ".";
	try
	{
		readSector(0, (Sector*)&_vhd);
		initialize_vhd(diskName, ownerName);
		writeSector(0, (Sector*)&_vhd);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
	_vhd._isFormated = false;
}

fstream* Disk::getdskfl()
{
	if (!_dskfl)
		return &_dskfl;
	else
		return NULL;
}

void Disk::seekToSector(unsigned int sectorNum)
{
	if (sectorNum > _vhd._ClusQty * 2 - 1)
		throw exception("Error: no such sector.");
	_dskfl.seekg(sectorNum * sizeof(Sector));
	_dskfl.seekp(sectorNum * sizeof(Sector));
	_currDiskSectorNr = sectorNum;
}

void Disk::writeSector(unsigned int sectorNum, Sector* buffPtr)
{
	try
	{
		seekToSector(sectorNum);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
	_dskfl.write((char*)buffPtr, sizeof(Sector));
	try
	{
		promote_currDiskSectorNr();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
}

void Disk::writeSector(Sector* buffPtr)
{
	_dskfl.write((char*)buffPtr, sizeof(Sector));
	try
	{
		promote_currDiskSectorNr();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
}

void Disk::readSector(unsigned int sectorNum, Sector* buffPtr)
{
	try
	{
		seekToSector(sectorNum);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
	_dskfl.read((char*)buffPtr, sizeof(Sector));
	try
	{
		promote_currDiskSectorNr();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
}

void Disk::readSector(Sector* buffPtr)
{
	_dskfl.read((char*)buffPtr, sizeof(Sector));
	try
	{
		promote_currDiskSectorNr();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
			// additional methods
void Disk::promote_currDiskSectorNr()
{
	try
	{
		if (_currDiskSectorNr < _vhd._ClusQty * 2 - 1)
			seekToSector(_currDiskSectorNr + 1);
		else
			seekToSector(0);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw exc;
	}
}
void Disk::initialize_vhd(string& diskName, string& ownerName)
{
	_vhd._sectorNr = 0;	//	by default the vhd is always in sector 0
	_vhd._ClusQty = 1600;
	_vhd._dataClusQty = 1600 - 4;	// (1 cluster for vhd and DAT + 1 cluster for Root directory) * 2 for backup = 4 non-data clusters
	_vhd._addrDAT = 1;	// address of DAT sector
	_vhd._addrRootDir = 1;	 // address of Root Directory cluster
	_vhd._addrDATcpy = 3198;
	_vhd._addrRootDirCpy = 1598;	// sectors no. 3196-3197
	_vhd._addrDataStart = 2;	// clusters 0-1 are vhd, DAT and Root Directory
	strcpy_s(_vhd._diskName, diskName.c_str());
	strcpy_s(_vhd._diskOwner, ownerName.c_str());
	strcpy_s(_vhd._prodDate, getCurrentDate().c_str());	// formatDate returns string object
	strcpy_s(_vhd._formatDate, "");
	strcpy_s(_vhd._emptyArea, "");
}
/*-------------------------------------------------------------------*/
							//	Level 1

void Disk::format(string& ownerName)
{
	if (strcmp(ownerName.c_str(), _vhd._diskOwner) != 0)
		throw exception("Error: Only the owner is able to perform disk format.");
	//	initialize DAT
	_dat._DAT.set();	// all bits are set to 1
	_dat._DAT.set(0, 0);	// vhd and dat cluster is set to 0
	_dat._DAT.set(_vhd._addrRootDir, 0);	// rootDir cluster is set to 0
	// set bits of backup clusters
	_dat._DAT.set(_vhd._addrRootDirCpy, 0);	// rootDir backup cluster is set to 0
	_dat._DAT.set(_vhd._addrRootDirCpy + 1, 0);	//	vhd and dat backup cluster is set to 0

	//	initialize Root Directory
	for (int i = 0; i < 17; i++)
		_rootDir._msbSector._dirEntry[i]._entryStatus = '0';
	_vhd._isFormated = true;
}

unsigned int Disk::howMuchEmpty()
{
	try
	{
		return _dat._DAT.count();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

void Disk::alloc(DATtype& fat, unsigned int numOfSectors, unsigned int algorithm)
{
	unsigned int numOfClusters = unsigned int(ceil(double(numOfSectors) / 2));
	try
	{
		switch (algorithm)
		{
		case 0:
			first_fit(fat, numOfClusters);
			break;
		case 1:
			best_fit(fat, numOfClusters);
			break;
		case 2:
			worst_fit(fat, numOfClusters);
			break;
		default:
			throw exception("Error: No such algorithm.");
			break;
		}
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

void Disk::first_fit(DATtype& fat, unsigned int numOfClusters, unsigned int currentCluster)
{
	if (numOfClusters == 0)
		return;
	unsigned int size = 0, firstFreeCluster = UINT_MAX;
	bool found = false;

	while (currentCluster != _vhd._ClusQty && !found)
	{
		if (_dat._DAT.test(currentCluster) == 1)
		{
			if (_dat._DAT.test(currentCluster) == 1 && firstFreeCluster > _vhd._ClusQty )
				firstFreeCluster = currentCluster;
			try
			{
				size = countSingleBlock(currentCluster);
			}
			catch (exception exc)
			{
				cout << exc.what() << endl;
				throw;
			}
			if (size >= numOfClusters)
				found = true;
		}
		else
			currentCluster++;
	}
//	allocation
	try
	{
		if (found)
			continuousAlloc(fat, currentCluster, numOfClusters);
		else
		{
			continuousAlloc(fat, firstFreeCluster, 1);	//	allocate the first free cluster
			first_fit(fat, numOfClusters - 1, currentCluster);
		}
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

void Disk::continuousAlloc(DATtype& fat, unsigned int startIndex, unsigned int sizeToAlloc)
{
	for (unsigned int i = startIndex; i < startIndex + sizeToAlloc; i++)
	{
		bool val = _dat._DAT[i] & fat[i];
		_dat._DAT.set(i, val);
		fat.set(i);
	}
}

void Disk::best_fit(DATtype& fat, unsigned int numOfClusters, unsigned int currentCluster)
{
	if (numOfClusters == 0)
		return;
	unsigned int size = 0, bestStartIndex, startFromCluster = currentCluster;
	bool foundEqual = false, foundLarger = false;

	while (currentCluster != _vhd._ClusQty && !foundEqual)
	{
		if (_dat._DAT.test(currentCluster) == 1)
		{
			size = countSingleBlock(currentCluster);
			currentCluster += size;
			if (!foundEqual && size == numOfClusters)
				foundEqual = true;
			if (!foundLarger && size > numOfClusters)
				foundLarger = true;
		}
		else
			currentCluster++;
	}
//	allocation
	try
	{
		if (foundEqual)
		{
			bestStartIndex = currentCluster - size;
			continuousAlloc(fat, bestStartIndex, size);
		}
		else if (foundLarger)
		{
			bestStartIndex = findBest(startFromCluster, numOfClusters, 'l');
			size = countSingleBlock(bestStartIndex);
			continuousAlloc(fat, bestStartIndex, numOfClusters);
		}
		else    //	all blocks are smaller than required
		{
			bestStartIndex = findBest(startFromCluster, numOfClusters, 's');
			unsigned int remainder = numOfClusters - size;
			continuousAlloc(fat, bestStartIndex, size);
			best_fit(fat, remainder, bestStartIndex + size - 1);
		}
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

unsigned int Disk::findBest(unsigned int currentCluster, unsigned int numOfClusters, char code)
{
//	This is an help method for best_fit algorithm.
//	It searches the DAT for the best block, i.e.
//	with the minimal difference between the size of
//	a free block and the size of the requested block
//	It has 2 modes - 'l' which stands for larger, i.e.
//	search for a block that larger than requsted, or
//	's' which stands for smaller than requested.
	if (code != 'l' && code != 's')
		throw exception("Error. Please choose 's' for smaller or 'l' for larger.");

	unsigned int startIndex, size, tempDiff, minDiff = UINT_MAX;

	while (currentCluster != _vhd._ClusQty)
	{
		if (_dat._DAT.test(currentCluster) == 1)
		{
			try
			{
				size = countSingleBlock(currentCluster);
			}
			catch (exception exc)
			{
				cout << exc.what() << endl;
				throw;
			}
			if ((size > numOfClusters && code == 'l') || (size < numOfClusters && code == 's'))
			{
				tempDiff = abs((int)(size - numOfClusters));
				if (tempDiff < minDiff)
				{
					minDiff = tempDiff;
					startIndex = currentCluster;
				}
			}
			currentCluster += size;
		}
		else
			currentCluster++;
	}

	return startIndex;
}

void Disk::worst_fit(DATtype& fat, unsigned int numOfClusters, unsigned int currentCluster)
{
	if (numOfClusters == 0)
		return;
//	else, i.e. if there is still to allocate - search for the largest block
	unsigned int tempSize = 0, maxSize = 0, maxStartIndex;
	while (currentCluster != _vhd._ClusQty)
	{
		if (_dat._DAT.test(currentCluster) == 1)
		{
			try
			{
				tempSize = countSingleBlock(currentCluster);
			}
			catch (exception exc)
			{
				cout << exc.what() << endl;
				throw;
			}
			if (tempSize > maxSize)
			{
				maxSize = tempSize;
				maxStartIndex = currentCluster;
			}
			currentCluster += tempSize;
		}
		else
			currentCluster++;

		tempSize = 0;
	}
	//	allocation
	try
	{
		if (maxSize >= numOfClusters)
			continuousAlloc(fat, maxStartIndex, numOfClusters);
		else    //	if the largest block is smaller than required
		{
			unsigned int remainder = numOfClusters - maxSize;
			continuousAlloc(fat, maxStartIndex, maxSize);
			worst_fit(fat, remainder);
		}
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

unsigned int Disk::countSingleBlock(unsigned int currentCluster, unsigned int count)
{
//	The method receives currentCluster which is a first free
//	cluster in a block. Then it counts recursively every free
//	cluster in this block, and returns the amount of free clusters.

	if (_dat._DAT.test(currentCluster) == 0 || currentCluster == _vhd._ClusQty)
		return count;
//	 i.e the current cluster is occupied or
//   the end of DAT has been reached (there are ClusQty - 1 clusters)

	else    //	continue counting
		return countSingleBlock(++currentCluster, ++count);
}

void Disk::allocextend(DATtype& fat, unsigned int numOfSectors, unsigned int algorithm)
{
	unsigned int numOfClusters = unsigned int(ceil(double(numOfSectors) / 2));
	unsigned int currentCluster;
	try
	{
		currentCluster = findEndOfFat(fat);
		switch (algorithm)
		{
		case 0:
			first_fit(fat, numOfClusters, currentCluster);
			break;
		case 1:
			best_fit(fat, numOfClusters, currentCluster);
			break;
		case 2:
			worst_fit(fat, numOfClusters, currentCluster);
			break;
		default:
			throw exception("Error: No such algorithm.");
			break;
		}
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

unsigned int Disk::findEndOfFat(DATtype& fat)
{
//	The method finds the end of the FAT, i.e. the index of
//	the 1st free cluster. The variable currentCluster will
//	have its value at the end of execution.
	unsigned int currentCluster = _vhd._ClusQty - 1;
	while (currentCluster != 0 && fat.test(currentCluster) != 1)
		currentCluster--;
	if (currentCluster == _vhd._ClusQty)
		throw exception("Error: Not enough place to extend file.");
	if (currentCluster != 0)
		currentCluster++;

	return currentCluster;
}

void Disk::dealloc(DATtype& fat)
{
	_dat._DAT |= fat;
}
/*-----------------------------------------------------------------------------*/
							//	Level 2

void Disk::createFile(string& fileName, string& user, unsigned int recSize, unsigned int numOfSectors, unsigned int keyOffset, unsigned int algorithm)
{
	DATtype fat;
	bool enoughSpace, freeEntryFound = false, exists = false;
	unsigned int i = 0, numOfClusters = unsigned int(ceil(double(numOfSectors) / 2));
	try
	{
		enoughSpace = numOfClusters <= howMuchEmpty();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
	if (!enoughSpace)
		throw exception("Error: Not enough space to perform allocation.");
	try
	{
		alloc(fat, numOfSectors, algorithm);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}

	int firstFreeEntry = -1;
	while (!exists && freeEntryFound == false && i < 17)
	{
		exists = strcmp(fileName.c_str(), _rootDir._lsbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._lsbSector._dirEntry[i]._entryStatus == '1';
		freeEntryFound = firstFreeEntry == -1 && _rootDir._lsbSector._dirEntry[i]._entryStatus != '1';
		if (freeEntryFound)
			firstFreeEntry = i;
		i++;
	}
	if (freeEntryFound == false)
		i = 0;
	while (!exists && freeEntryFound == false && i < 17)
	{
		exists = strcmp(fileName.c_str(), _rootDir._msbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._msbSector._dirEntry[i]._entryStatus == '1';
		freeEntryFound = firstFreeEntry == -1 && _rootDir._lsbSector._dirEntry[i]._entryStatus != '1';
		if (freeEntryFound)
			firstFreeEntry = i;
		i++;
	}
	if (exists)
		throw exception("Error: File with such name already exits in this folder.");
	if (firstFreeEntry == -1)
		throw exception("Error: Maximum number of files in the directory has been reached.");

//	at this point file can be created
	DirEntry entry;
	strcpy_s(entry._fileName, fileName.c_str());
	strcpy_s(entry._fileOwner, user.c_str());
	i = 0;
	while (fat.test(i) == 0)
		i++;
	entry._fileAddr = i * 2;
	strcpy_s(entry._crDate, getCurrentDate().c_str());
	entry._eofRecNr = 0;
	entry._recSize = recSize;
	entry._fileSize = numOfSectors;
	entry._keyOffset = keyOffset;
	entry._entryStatus = '1';

	if (firstFreeEntry < 17)
		_rootDir._lsbSector._dirEntry[firstFreeEntry] = entry;
	else
		_rootDir._msbSector._dirEntry[firstFreeEntry] = entry;

	FileHeader header;
	header._FAT = fat;
	header._fileDesc = entry;
	header._sectorNr = entry._fileAddr;
	try
	{
		writeSector(entry._fileAddr, (Sector*)&header);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

void Disk::delFile(string& fileName, string& user)
{
	int i;
	bool found = false;
	for (i = 0; i < 17 && !found; i++)
	{
		found = strcmp(fileName.c_str(), _rootDir._lsbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._lsbSector._dirEntry[i]._entryStatus == '1';
	}
	if (!found)
		i = 0;
	while (!found && i < 17)
	{
		found = strcmp(fileName.c_str(), _rootDir._msbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._msbSector._dirEntry[i]._entryStatus == '1';
		i++;
	}

	if (!found)
		throw exception("Error. File doesn't exist in the directory.");
	
//	file was found
	DirEntry* entryPtr;
	if (i < 17)
		entryPtr = &_rootDir._lsbSector._dirEntry[i-1];
	else
		entryPtr = &_rootDir._msbSector._dirEntry[i-1];

	if (strcmp(user.c_str(), entryPtr->_fileOwner) != 0)
		throw exception("Error. Only the owner can delete the file.");

//	at this point file can be deleted
	entryPtr->_entryStatus = '2';
	FileHeader* headerPtr = new FileHeader;
	try
	{
		readSector(entryPtr->_fileAddr, (Sector*)headerPtr);
		dealloc(headerPtr->_FAT);
		delete headerPtr;
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

void Disk::extendFile(string& fileName, string& user, unsigned int numOfSectors, unsigned int algorithm)
{
	DATtype fat;
	bool enoughSpace, found = false;
	unsigned int i, numOfClusters = unsigned int(ceil(double(numOfSectors) / 2));
	try
	{
		enoughSpace = numOfClusters <= howMuchEmpty();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
	if (!enoughSpace)
		throw exception("Error: Not enough space to extend the file.");
	for (i = 0; i < 17 && !found; i++)
	{
		found = strcmp(fileName.c_str(), _rootDir._lsbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._lsbSector._dirEntry[i]._entryStatus == '1';
	}
	if (!found)
		i = 0;
	while (!found && i < 17)
	{
		found = strcmp(fileName.c_str(), _rootDir._msbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._msbSector._dirEntry[i]._entryStatus == '1';
		i++;
	}

	if (!found)
		throw exception("Error. File doesn't exist in the directory.");

//	file was found
	DirEntry* entryPtr;
	if (i < 17)
		entryPtr = &_rootDir._lsbSector._dirEntry[i-1];
	else
		entryPtr = &_rootDir._msbSector._dirEntry[i-1];

	if (strcmp(user.c_str(), entryPtr->_fileOwner) != 0)
		throw exception("Error. Only the owner can extend the file.");

//	at this point file can be extended
	FileHeader* headerPtr = new FileHeader;
	try
	{
		readSector(entryPtr->_fileAddr, (Sector*)headerPtr);
		headerPtr->_fileDesc._fileSize += numOfSectors;
		allocextend(headerPtr->_FAT, numOfSectors, algorithm);
		writeSector(entryPtr->_fileAddr, (Sector*)headerPtr);

		delete headerPtr;
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}
/*-----------------------------------------------------------------------------*/
							//	Level 3	

FCB* Disk::openFile(string& fileName, string& user, string& openMode)
{
	int i;
	bool found = false;
	for (i = 0; i < 17 && !found; i++)
	{
			found = strcmp(fileName.c_str(), _rootDir._lsbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._lsbSector._dirEntry[i]._entryStatus == '1';
	}
	if (!found)
		i = 0;
	while (!found && i < 17)
	{
		found = strcmp(fileName.c_str(), _rootDir._msbSector._dirEntry[i]._fileName) == 0
			&& _rootDir._msbSector._dirEntry[i]._entryStatus == '1';
		i++;
	}

	if (!found)
		throw exception("Error. File doesn't exist in the directory.");

//	file was found
	DirEntry* entryPtr;
	if (i < 17)
		entryPtr = &_rootDir._lsbSector._dirEntry[i-1];
	else
		entryPtr = &_rootDir._msbSector._dirEntry[i-1];
	
	FileHeader header;
	try
	{
		readSector(entryPtr->_fileAddr, (Sector*)&header);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}

	FCB* fcbPtr = new FCB(this);
	fcbPtr->_currRecNrInBuff = 0;
	fcbPtr->_fileDesc = header._fileDesc;
	fcbPtr->_FAT = header._FAT;
	fcbPtr->_openMode = openMode;
	fcbPtr->_maxNumOfRec = unsigned int(floor(double((sizeof(Sector)-sizeof(unsigned int))) / fcbPtr->_fileDesc._recSize));
	if (openMode == "I" || openMode == "O" || openMode == "IO")
	{
		if (openMode != "I" && strcmp(user.c_str(), fcbPtr->_fileDesc._fileOwner) != 0)
			throw exception("Error. Only the owner can open for this mode.");
		fcbPtr->_currRecNr = 0;
		fcbPtr->_currSecNr = 1;	//	sector no. 0 is always the file header
//	the 1st sector of data will be always after the header, in the same cluster
		fcbPtr->_currSecAddr = fcbPtr->_fileDesc._fileAddr + 1;
	}
	else if (openMode == "E")
	{
		fcbPtr->_currRecNrInBuff = (fcbPtr->_fileDesc._eofRecNr - 1) %  fcbPtr->_maxNumOfRec;
		fcbPtr->_currRecNr = fcbPtr->_fileDesc._eofRecNr - 1;
		fcbPtr->_currSecNr = fcbPtr->_fileDesc._fileSize - 1;
		fcbPtr->_currSecAddr = fcbPtr->getSecAddr(fcbPtr->_currSecNr);
	}
	else
		throw exception("Error. Please choose \"I\" for input, \"O\" for output, or \"IO\" for both.");
//	read the relevant sector to the buffer
	readSector(fcbPtr->_currSecAddr, &(fcbPtr->_buffer));

	return fcbPtr;
}


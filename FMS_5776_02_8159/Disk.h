#pragma once
/*******************************************************************\
File: Disk.h ;    Path: FMS_5776_02_8159\Disk.h
\*******************************************************************/

#include <iostream>
#include <fstream>
#include "DAT.h"
#include "Dir.h"
#include "FileHeader.h"
#include "Sector.h"
#include "VolumeHeader.h"
using namespace std;
class Disk
{
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
																//	Level 0
// fields
	VolumeHeader _vhd;
	DAT _dat;
	RootDir _rootDir;
	bool _mounted;
	fstream _dskfl;
	unsigned int _currDiskSectorNr;
// methods
public:
	//CTORs
	Disk(); // default CTOR
	Disk(string&, string&, char);
	//DTOR
	~Disk();
private:
	void createdisk(string&, string&);	//	initializes the disk handle to a new or to an existing virtual disk
	void mountdisk(string&);	//	mounts an existing virtual disk to the handle
	void unmountdisk();	//	unmounts the current disk
	void recreatedisk(string&, string&);	//	initialize the disk from the beginning
	fstream* getdskfl();	//	returns a pointer to the disk handle
	void seekToSector(unsigned int);	//	seeks the requested sector
	void writeSector(unsigned int, Sector*);	//	writes a sector to the disk to requested place
	void writeSector(Sector*);	//	writes a sector to the disk to the next sector
	void readSector(unsigned int, Sector*);	//	reads the requsted sector from the disk
	void readSector(Sector*);	//	reads the next sector from the disk

	friend class TestLevel_0;
	friend class TestLevel_1;
	friend class TestLevel_2;
	friend class TestLevel_3;
	friend class TestLevel_4;
	friend class FCB;
	friend class DMS;
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
				// Extras
	void promote_currDiskSectorNr();	// promotes _currDiskSectorNr if i'ts not last, if it is - return _currDiskSectorNr to beginning
	void initialize_vhd(string&, string&);

		//	End of Level 0
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
																//	Level 1
	void format(string&);
	unsigned int howMuchEmpty();
	void alloc(DATtype&, unsigned int, unsigned int = 0);
	void allocextend(DATtype&, unsigned int, unsigned int = 0);
	void dealloc(DATtype&);
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
				// Extras
	void first_fit(DATtype&, unsigned int, unsigned int = 0);	//	Allocation algorithm, for new or extending allocation
	void best_fit(DATtype&, unsigned int, unsigned int = 0);	//	Allocation algorithm, for new or extending allocation
	void worst_fit(DATtype&, unsigned int, unsigned int = 0);	//	Allocation algorithm, for new or extending allocation
	unsigned int countSingleBlock(unsigned int, unsigned int = 0);		//	Counts the number of free clusters in a single block in the DAT
	void continuousAlloc(DATtype&, unsigned int, unsigned int);		//	Allocates clusters continuously
	unsigned int findBest(unsigned int, unsigned int, char);			//	Finds a block with the minimal difference from requested
	unsigned int findEndOfFat(DATtype&);		//	Finds the first free cluster in the end of the FAT.

		//	End of Level 1
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
																//	Level 2

	void createFile(string&, string&, unsigned int, unsigned int, unsigned int, unsigned int);	//	creates a new file
	void delFile(string&, string&);	//	deletes the requsted file
	void extendFile(string&, string&, unsigned int, unsigned int);	//	extends the requested file

		//	End of Level 2
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
																//	Level 3

	FCB* openFile(string&, string&, string&);	//	initialize the handle to the file
};
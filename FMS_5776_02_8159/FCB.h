#pragma once
/*******************************************************************\
File: FCB.h  ;    Path: FMS_5776_02_8159\FCB.h
\*******************************************************************/
#include "Disk.h"

class FCB
{
// fields
	Disk* _d;
	DirEntry _fileDesc;
	DATtype _FAT;
	Sector _buffer;
	unsigned int _currRecNr = UINT_MAX;
	unsigned int _currSecNr = UINT_MAX;
	unsigned int _currRecNrInBuff = UINT_MAX;
// methods
public:
	//CTORs
	FCB(){ _d = NULL; }	//	default CTOR
	FCB(Disk* disk){ _d = disk; }
//	~FCB();
private:
	void closeFile();	//	close the handle to the file
	void flushFile();	//	write the sector that currently in the buffer to the file
	void readRec(char*, unsigned int = 0);	//	read the current record from the buffer
	void writeRec(char*);	//	write the given record to the buffer
	void seekRec(unsigned int, int);	//	seek a record in the file
	void updateCancel();	//	end the update process
	void deleteRec();	//	delete current record
	void updateRec(char*);	//	update current record

	friend class Disk;
	friend class DMS;
	friend class TestLevel_3;
	friend class TestLevel_4;
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
				// Extras
// fields
	bool _bufferChanged = false;	//	a flag that indicates whether buffer changed
	string _openMode = "I";	//	saves the requested open mode
	int _lockedRec = -1;
	unsigned int _currSecAddr = UINT_MAX;	//	adress of _currSecNr
	unsigned int _maxNumOfRec = UINT_MAX;	// max num of rec in a sector
// methods
	unsigned int findNextSecAddr(unsigned int);	//	returns the adress of the next sector of the file, after the given adress
	unsigned int getSecAddr(unsigned int);	//	returns the adress of a given sector
	void checkIfLocked();	//	checks if already finished updating
	unsigned int getSecNrOfRec(unsigned int);	//	returns the sector number of the record
};
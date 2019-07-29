#pragma once
/*******************************************************************\
File: DMS.h  ;    Path: FMS_5776_02_8159\DMS.h
\*******************************************************************/
#include "FCB.h"

class DMS
{
// fields
	FCB* _fcbArray;
	unsigned int _fcbArrSize;
//	methods
public:
	//CTORs and DTOR
	DMS();	//	default CTOR
	DMS(unsigned int);	//	CTOR
	~DMS();	//	DTOR
private:
	FCB* openFile(Disk*, string&, string&, string&);
	FCB* lookForFCB(Disk*, string&);
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
				// Extras
	void unmount(Disk*);	//	closes all files associated with the given disk and umnounts the disk

	friend class TestLevel_4;
};
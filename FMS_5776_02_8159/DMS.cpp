#pragma once
/*******************************************************************\
File: DMS.cpp  ;    Path: FMS_5776_02_8159\DMS.cpp
\*******************************************************************/
#include "DMS.h"

DMS::DMS()
{
	_fcbArray = new FCB[5];
	_fcbArrSize = 5;
}

DMS::DMS(unsigned int size)
{
	_fcbArray = new FCB[size];
	_fcbArrSize = size;
}

/*DMS::~DMS()
{
	Disk* d;
	for (int i = 0; i < _fcbArrSize; i++)
	{
		d = _fcbArray[i]._d;
		if (d != NULL)
			unmount(d);
	}
																														
	delete _fcbArray;
}*/
DMS::~DMS(){}

FCB* DMS::openFile(Disk* d, string& fileName, string& user, string& openMode)
{
	FCB* freeFCB;
	try
	{
		freeFCB = lookForFCB(d, fileName);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
	if (freeFCB == NULL)
		throw exception("Error: No free FCB. Please close some file.");
	else
		freeFCB->_d = d;
	try
	{
		*freeFCB = *freeFCB->_d->openFile(fileName, user, openMode);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}

	return freeFCB;
}
FCB* DMS::lookForFCB(Disk* d, string& fileName)
{
	bool isOpen = false;
	int index = -1;
	for (int i = 0; i < _fcbArrSize && isOpen == false; i++)
	{
		if (index == -1 && _fcbArray[i]._d == NULL)
			index = i;
		else
		{
			if (string(_fcbArray[i]._fileDesc._fileName) == fileName && _fcbArray[i]._d == d)
				isOpen == true;
		}
	}

	if (isOpen)
		throw exception("Error: Such file is already open in the disk.");
	if (index == -1)
		return NULL;
	else
		return &_fcbArray[index];
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
				// Extras
void DMS::unmount(Disk* d)
{
	for (int i = 0; i < _fcbArrSize; i++)
	{
		if (_fcbArray[i]._d == d)
			_fcbArray[i].closeFile();
	}
	d->unmountdisk();
}

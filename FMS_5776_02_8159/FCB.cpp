#pragma once
/*******************************************************************\
File: FCB.cpp  ;    Path: FMS_5776_02_8159\FCB.cpp
\*******************************************************************/
#include "FCB.h"

void FCB::closeFile()
{
	checkIfLocked();
	try
	{
		flushFile();
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
//	update header
	FileHeader header;
	header._FAT = _FAT;
	header._fileDesc = _fileDesc;
	header._sectorNr = _fileDesc._fileAddr;
	try
	{
		_d->writeSector(header._sectorNr, (Sector*)&header);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
//	free FCB
	_d = NULL;
	_currRecNr = _currRecNrInBuff = _currSecNr = 0;
}
void FCB::flushFile()
{
	checkIfLocked();
	if (_bufferChanged == false)
		return;

	try
	{
		_d->writeSector(_currSecAddr, &_buffer);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
	_currRecNrInBuff = 0;
	_bufferChanged = false;
}

void FCB::readRec(char* recPtr, unsigned int toUpdate)
{
	checkIfLocked();
	if (_openMode == "O" || _openMode == "E")
		throw exception("Error: Can't read. The file was opened only for writing.");
	if (_openMode == "I" && toUpdate == 1)
		throw exception("Error: For update open in IO mode.");
	try
	{
		if (_currRecNrInBuff == _maxNumOfRec)
		{
			if (_currSecNr == _fileDesc._fileSize - 1)
				throw exception("Error: Can't read, end of file has been reached.");
			_currSecAddr = findNextSecAddr(_currSecAddr);
			_d->readSector(_currSecAddr, &_buffer);
			_bufferChanged = true;
			_currSecNr++;
			_currRecNrInBuff = 0;
		}
		memcpy(recPtr, (char*)&_buffer + _currRecNrInBuff * _fileDesc._recSize, _fileDesc._recSize);
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}

	if (_openMode == "IO" && toUpdate == 1)
		_lockedRec = _currRecNr;
	else
	{
		_currRecNrInBuff++;
		_currRecNr++;
	}
}

void FCB::writeRec(char* recPtr)
{
	checkIfLocked();
	if (_openMode == "I")
		throw exception("Error: Can't write. The file was opened only for reading.");

	try
	{
		if (_currRecNrInBuff == _maxNumOfRec)
		{
			if (_currSecNr == _fileDesc._fileSize - 1)
				throw exception("Error: Can't write, end of file has been reached. Please extend the file.");
			flushFile();
			_currSecAddr = findNextSecAddr(_currSecAddr);
			_currSecNr++;
			_currRecNrInBuff = 0;
		}
		memcpy((char*)&_buffer + _currRecNrInBuff * _fileDesc._recSize, recPtr, _fileDesc._recSize);
		_bufferChanged = true;
		_currRecNrInBuff++;
		_currRecNr++;
	}
	catch (exception exc)
	{
		cout << exc.what() << endl;
		throw;
	}
}

void FCB::seekRec(unsigned int jumpMode, int jumpLength)
{
	checkIfLocked();
	if (jumpLength > 0 && jumpMode == 2 || jumpLength < 0 && jumpMode == 0)
		throw exception("Error: This jump is illegal.");

	int destRec;
	if (jumpMode == 0)
		destRec = jumpLength;
	else if (jumpMode == 1)
		destRec = _currRecNr + jumpLength;
	else if (jumpMode == 2)
		destRec = _fileDesc._eofRecNr + jumpLength;
	else
		throw exception("Error: There is no such jump mode.");

	if (destRec >= _fileDesc._eofRecNr || destRec < 0)
		throw exception("Error: There is no such record.");

	_currRecNr = destRec;
	unsigned int newSecNr = getSecNrOfRec(_currRecNr);

	if (newSecNr != _currSecNr)
	{
		try
		{
			flushFile();
		}
		catch (exception exc)
		{
			cout << exc.what() << endl;
			throw;
		}
		_currSecNr = newSecNr;
		_currSecAddr = getSecAddr(_currSecNr);
		_d->readSector(_currSecAddr, &_buffer);
	}
	_currRecNrInBuff = _currRecNr % _maxNumOfRec;
}

void FCB::updateCancel()
{
	if (_lockedRec == -1)
		throw exception("Error: The record is not locked.");
	if (_openMode != "IO")
		throw exception("Error: The file isn't opened for IO.");

	_lockedRec = -1;
}

void FCB::deleteRec()
{
	if (_lockedRec == -1)
		throw exception("Error: The record is not locked.");
	if(_openMode != "IO")
		throw exception("Error: The file isn't opened for IO.");

	if (_currRecNrInBuff == _maxNumOfRec)
	{
		if (_currSecNr < _fileDesc._fileSize - 1)
			throw exception("Error: Can't delete, end of file has been reached.");
		flushFile();
		_currSecAddr = findNextSecAddr(_currSecAddr);
		_currSecNr++;
	}
	int* zero = new int;
	*zero = 0;
	memcpy((char*)&_buffer + _currRecNrInBuff * _fileDesc._recSize, zero, sizeof(int));
	_bufferChanged = true;
	_fileDesc._eofRecNr--;
	_currRecNrInBuff++;
	_currRecNr++;
	_lockedRec = -1;

	delete zero;
}

void FCB::updateRec(char* recPtr)
{
	if (_lockedRec == -1)
		throw exception("Error: The record is not locked.");
	if (_openMode != "IO")
		throw exception("Error: The file isn't opened for IO.");

	if (_currRecNrInBuff == _maxNumOfRec)
	{
		if (_currSecNr < _fileDesc._fileSize - 1)
			throw exception("Error: Can't update, end of file has been reached.");
		flushFile();
		_currSecAddr = findNextSecAddr(_currSecAddr);
		_currSecNr++;
	}
	memcpy((char*)&_buffer + _currRecNrInBuff * _fileDesc._recSize, recPtr, _fileDesc._recSize);
	_bufferChanged = true;
	_currRecNrInBuff++;
	_currRecNr++;
	_lockedRec = -1;
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
			// additional methods
unsigned int FCB::findNextSecAddr(unsigned int secAddr)
{
	if (secAddr % 2 == 0)
		return secAddr + 1;	//	next sector is in the same cluster
//	else, search for the next cluster allocated to the file
	unsigned int clusAddr = unsigned int(floor(double(secAddr) / 2));
	bool foundNextSec = false;
	while (!foundNextSec)
	{
		clusAddr++;
		if (_FAT.test(clusAddr) == 1)
			foundNextSec = true;
	}
	secAddr = clusAddr * 2;

	return secAddr;
}

unsigned int FCB::getSecAddr(unsigned int secNr)
{
	unsigned int i = 0, addr = _fileDesc._fileAddr;
	while (i < secNr)
	{
		try
		{
			addr = findNextSecAddr(addr);
		}
		catch (exception exc)
		{
			cout << exc.what() << endl;
			throw;
		}
		i++;
	}

	return addr;
}

void FCB::checkIfLocked()
{
	if (_lockedRec != -1)
	{
		string str = string("Error: Record number ")+ to_string(_lockedRec) + string(" is locked.");
		throw str;
	}
}

unsigned int FCB::getSecNrOfRec(unsigned int recNr)
{
	double maxNumOfRec;	//	max number of records in 1 sector
	maxNumOfRec = floor(double((sizeof(Sector)-sizeof(unsigned int))) / _fileDesc._recSize);
	unsigned int secNr = unsigned int(floor(double(recNr) / maxNumOfRec)) + 1;	//	sector number which contains _currRecNr, +1 for the header

	return secNr;
}
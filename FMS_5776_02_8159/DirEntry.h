#pragma once
/*******************************************************************\
File: DirEntry.h ;    Path: FMS_5776_02_8159\DirEntry.h
Description:
\*******************************************************************/

using namespace std;
class DirEntry
{
	char _fileName[12];
	char _fileOwner[12];
	unsigned int _fileAddr = UINT_MAX;
	char _crDate[11];
	unsigned int _fileSize = UINT_MAX;
	unsigned int _eofRecNr = UINT_MAX;
	unsigned int _recSize = UINT_MAX;
	unsigned int _keyOffset = UINT_MAX;
	unsigned char _entryStatus = UCHAR_MAX;

	friend class Disk;
	friend class SectorDir;
	friend class FileHeader;
	friend class FCB;
	friend class DMS;
	friend class TestLevel_3;
	friend class TestLevel_4;
public:
	DirEntry::DirEntry()
	{
		strcpy_s(_fileName, "");
		strcpy_s(_fileOwner, "");
		strcpy_s(_crDate, "");
	}
};
#pragma once
/*******************************************************************\
File: FileHeader.h ;    Path: FMS_5776_02_8159\FileHeader.h
\*******************************************************************/


using namespace std;
class FileHeader
{
	unsigned int _sectorNr;
	DirEntry _fileDesc;
	DATtype _FAT;
	char _emptyArea[760];

public:
	FileHeader(){ strcpy_s(_emptyArea, ""); }

	friend class Disk;
	friend class FCB;
	friend class TestLevel_4;
};
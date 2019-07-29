#pragma once
/*******************************************************************\
File: Dir.h ;    Path: FMS_5776_02_8159\Dir.h
\*******************************************************************/

#include "DirEntry.h"
using namespace std;
class SectorDir
{
	int _sectorNr;
	DirEntry _dirEntry[17];

	friend class Disk;
	friend class TestLevel_3;
	friend class TestLevel_4;
};

class RootDir
{
	SectorDir _msbSector;
	SectorDir _lsbSector;
public:
//	RootDir();
//	~RootDir();

	friend class Disk;
	friend class TestLevel_3;
	friend class TestLevel_4;
};
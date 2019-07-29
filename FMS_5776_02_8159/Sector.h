#pragma once
/*******************************************************************\
File: Sector.h ;    Path: FMS_5776_02_8159\Sector.h
\*******************************************************************/

using namespace std;
class Sector
{
	unsigned int _sectorNr;
	char _rawData[1020];

public:
	Sector(){ _sectorNr = UINT_MAX; strcpy_s(_rawData, ""); }

	friend class Disk;
	friend class FCB;
	friend class TestLevel_0;
	friend class TestLevel_4;
};
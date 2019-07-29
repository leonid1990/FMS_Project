#pragma once
/*******************************************************************\
File: DAT.h  ;    Path: FMS_5776_02_8159\DAT.h
\*******************************************************************/

#include <bitset>
using namespace std;
typedef bitset<1600> DATtype;
class DAT
{
	unsigned int _sectorNr;
	DATtype _DAT;
	char _emptyArea[816];
public:
	DAT(){ _sectorNr = UINT_MAX; strcpy_s(_emptyArea, ""); }

	friend class Disk;
	friend class TestLevel_1;
	friend class TestLevel_2;
	friend class TestLevel_3;
	friend class TestLevel_4;
};
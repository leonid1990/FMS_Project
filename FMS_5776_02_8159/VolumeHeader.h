#pragma once
/*******************************************************************\
File: VolumeHeader.h ;    Path: FMS_5776_02_8159\VolumeHeader.h
Description: Volume Header of the virtual disk, contains various details.
\*******************************************************************/


#include "additionalFunctions.h"
using namespace std;
class VolumeHeader
{
// fields
	unsigned int _sectorNr = 0;
	char _diskName[12];
	char _diskOwner[12];
	char _prodDate[11];
	unsigned int _ClusQty = 0;
	unsigned int _dataClusQty = 0;
	unsigned int _addrDAT = 0;
	unsigned int _addrRootDir = 0;
	unsigned int _addrDATcpy = 0;
	unsigned int _addrRootDirCpy = 0;
	unsigned int _addrDataStart = 0;
	char _formatDate[11];
	bool _isFormated = 0;
	char _emptyArea[943];
// methods
public:
	//CTORs
	VolumeHeader();	//	default CTOR, assigns default values to the fields without connecting it to a file

	friend class Disk;
	friend class TestLevel_0;
};
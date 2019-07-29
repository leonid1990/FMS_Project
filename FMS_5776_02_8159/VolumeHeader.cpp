#pragma once
/*******************************************************************\
File: VolumeHeader.cpp ;    Path: FMS_5776_02_8159\VolumeHeader.cpp
Description: Implemnts VolumeHeader.h
\*******************************************************************/

#include "VolumeHeader.h"

VolumeHeader::VolumeHeader()
{
	strcpy_s(_diskName, "");
	strcpy_s(_diskOwner, "");
	strcpy_s(_prodDate, "");
	strcpy_s(_emptyArea, "");
}
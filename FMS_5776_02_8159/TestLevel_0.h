#pragma once
#include "Disk.h"
using namespace std;
class TestLevel_0
{

	static void printStructSize()
	{
		cout << "start" << endl;
		cout << "Size Of Sector -->" << sizeof(Sector) << endl;
		cout << "Size Of VolumeHeader -->" << sizeof(VolumeHeader) << endl;
		cout << "Size Of DAT -->" << sizeof(DAT) << endl;
		cout << "Size Of dir_Entry -->" << sizeof(DirEntry) << endl;
		cout << "Size Of SectorDir -->" << sizeof(SectorDir) << endl;
		cout << "Size Of FileHeader -->" << sizeof(FileHeader) << endl;
		cout << "Size Of rootDir -->" << sizeof(RootDir) << endl;
	}
	static void printDiskInfo(Disk & d)
	{

		VolumeHeader* vh = &d._vhd;
		cout << " disk name:        " << vh->_diskName << endl;
		cout << " Owner Name:       " << vh->_diskOwner << endl;
		cout << " prodDate:         " << vh->_prodDate << endl;
		cout << " formatDate:       " << vh->_formatDate << endl;
		cout << " isFormated:       " << vh->_isFormated << endl;
		cout << " addrDataStart:    " << vh->_addrDataStart << endl;
		cout << " ClusQty:          " << vh->_ClusQty << endl;
		cout << " dataClusQty:      " << vh->_dataClusQty << endl;
		cout << " addrDAT:          " << vh->_addrDAT << endl;
		cout << " addrrootDir:      " << vh->_addrRootDir << endl;
		cout << " addrDATcpy:       " << vh->_addrDATcpy << endl;
		cout << " addrrootDirCpy:   " << vh->_addrRootDir << endl << endl;
	}
	static void test_create(string diskName, string ownerName)
	{
		Disk d;
		cout << "\npre createdisk: " << endl;
		printDiskInfo(d);
		cout << "post createdisk: " << endl;
		d.createdisk(diskName, ownerName);
		printDiskInfo(d);
	}
	static void test_mount(string diskName)
	{
		Disk d;
		cout << "\npre mountdisk: " << endl;
		printDiskInfo(d);
		d.mountdisk(diskName);
		cout << "post mountdisk: " << endl;
		printDiskInfo(d);
		d.unmountdisk();
	}
	static void test_rwSector(string diskName)
	{
		Disk d;
		Sector sector;
		d.mountdisk(diskName);
		cout << "\nread sector: " << endl;
		d.readSector(8, &sector);
		strcpy_s(sector._rawData, "this is write temp sector");
		d.writeSector(8, &sector);
		d.unmountdisk();
	}
public:
	static void test_0()
	{
		try
		{
			string diskName = "disk 1";
			string ownerName = "Hugh";
			printStructSize();
			test_create(diskName, ownerName);
			test_mount(diskName);
			test_rwSector(diskName);
		}
		catch (exception ex)
		{
			cout << ex.what() << endl;
		}
	}
};
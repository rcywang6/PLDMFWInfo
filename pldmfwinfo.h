#ifndef _PLDM_FW_INFO_
#pragma once


#define E_UNKNOWN_PLDM_VERSION 0x90000001;
#define BIT0 0x00000001

#pragma pack(1) 


typedef unsigned char UUID[16];
typedef  unsigned char TimeStamp104[13];

enum {
	unknown_version=0,
	version_v10,
	version_v11,
};

enum  StringType {
	unknown, ASCII, UTF8, UTF16, UTF16LE, UTF16BE};

const UUID hdrid_v10 = { 0xf0,0x18,0x87,0x8c,0xcb,0x7d,0x49,0x43,0x98,0x00,0xa0,0x2f,0x05,0x9a,0xca,0x02 };
const UUID hdrid_v11 = { 0x12,0x44,0xd2,0x64,0x8d,0x7d,0x47,0x18,0xa0,0x30,0xfc,0x8a,0x56,0x58,0x7d,0x5a };

#define OFFSET_OF(a , b) ((int)(&(((a*)(0))->b))) 

typedef struct {
	UUID pkgHdrId; 
	unsigned char pkgHdrFmtRevision;
	unsigned short pkgHdrSize;
	TimeStamp104 pkgRelDateTime;
	unsigned short compBmpBitLength;
	unsigned char pkgVerStrType;
	unsigned char pkgVerStrLength; 
	unsigned char* pkgVerString;
} PackageHeaderInfo;

typedef struct {
	unsigned short iniDescType;
	unsigned short iniDescLength;
	unsigned char* iniDescData;
} RecordDescriptors;

typedef struct {
	unsigned short recLength; 
	unsigned char descCount;
	unsigned int devUpdateOptFlags; 
	unsigned char compImgSetVerStrType;
	unsigned char compImgSetVerStrLength;
	unsigned short fwDevPkgDataLength;
	unsigned char appComponents;
	unsigned char* compImgSetVerStr;
	RecordDescriptors* recDesc;
	unsigned char* fwDevPkgData;
} DeviceIDRecord;

typedef struct {
	unsigned short dsRecLength;
	unsigned char dsDescCount;
	unsigned int dsDevUpdateOptFlags;
	unsigned char dsActMinVerStrType;
	unsigned char dsActMinVerStrLength;
	unsigned short dsDevPkgDataLength;
	unsigned char dsAppComponents;
	unsigned char* dsActMinVerString;
	unsigned int dsDevActMinVerComStamp;   // dthe field depends on dsDevUpdateOptFlags
	RecordDescriptors* dsRecDesc;
	unsigned char* dsDevPkgData;
} DownstreamDeviceIDRecord;

typedef struct {
	unsigned short compClassfication;
	unsigned short compIdentifier;
	unsigned int compCompareStamp;
	unsigned short compOptions;
	unsigned short reqCompActMethod;
	unsigned int compLocOffset;
	unsigned int compSize;
	unsigned char compVerStrType;
	unsigned char compVerStrLength;
	unsigned char* compVerString;
} ComponentImageInfo;

typedef struct {
	PackageHeaderInfo* hdrInfo;
	unsigned char devIDRecCount; 
	DeviceIDRecord* devRecord;
	unsigned short compCount;
	ComponentImageInfo* compInfo;
	unsigned int checkSum;
	// DSP0267_1.1.0 Adds Downstream Device firmware update support
	unsigned char dsDevIDRecCount;
	DownstreamDeviceIDRecord* dsDevRecord;
} PackageHeader;

void PrintPackageHeader(PackageHeader* pph);
int ParsePackageHeader(unsigned char* fwHeader, PackageHeader* pPkgHeader);
void FreePackageHeader(PackageHeader* pPkg);
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pldmfwinfo.h"
 
int CheckFWUpgVersion(UUID* hdrInfo)
{
	if (memcmp(hdrInfo, hdrid_v10, 16) == 0)
		return version_v10;
	else if (memcmp(hdrInfo, hdrid_v11, 16) == 0)
		return version_v11;
	else 
		return unknown;
}

void PrintCompImageInfo(ComponentImageInfo* pComp)
{

	printf("\t \t \t Component Classfication %04x\n", pComp->compClassfication);
	printf("\t \t \t Component Identifier %04x\n", pComp->compIdentifier);
	printf("\t \t \t Component Identifier %08x\n", pComp->compCompareStamp);
	printf("\t \t \t Comp options: %02x \n", pComp->compOptions);
	printf("\t \t \t Requested Component Action: %02x \n", pComp->reqCompActMethod);
	printf("\t \t \t Component Location Offset: %d \n", pComp->compLocOffset);
	printf("\t \t \t Component size: %08x \n", pComp->compSize);
	printf("\t \t \t Component String Type: %x \n", pComp->compVerStrType);
	printf("\t \t \t Component Version String Length: %08x \n", pComp->compVerStrLength);
	printf("\t \t \t Component Version String: %s \n", pComp->compVerString);
}

void PrintPackageHeaderInfo(PackageHeaderInfo* hdr)
{
	int i;
	printf("Package Header Info: \n");
	printf("\t Package Header Identifier: ");
	for (i = 0; i < 16; i++)
		printf("%02x ", hdr->pkgHdrId[i]);
	printf("\n");

	printf("\t Package Header Format Revision: %d \n", hdr->pkgHdrFmtRevision);
	printf("\t Package Header Size: %d \n", hdr->pkgHdrSize);

	// TODO: implement date and time representatiion
	printf("\t Package Release Date Time: ");
	for (i = 0; i < 13; i++)
		printf("%02x", hdr->pkgRelDateTime[i]);
	printf("\n");

	printf("\t Component Bitmap Bit length: %d \n", hdr->compBmpBitLength);
	printf("\t Package String Verion Length: %d \n", hdr->pkgVerStrLength);
	printf("\t Package String Verion: %s \n", hdr->pkgVerString);
}

void PrintRecordDescriptor(RecordDescriptors* pDesc)
{
	printf("\t \t \t Descriptor type: %04x\n", pDesc->iniDescType);
	printf("\t \t \t Descriptor length: %04x\n", pDesc->iniDescLength);
	printf("\t \t \t Descriptor Data: %04x\n", pDesc->iniDescData[1] << 8 | pDesc->iniDescData[0]);
}

void PrintDeviceIDRecord(DeviceIDRecord* pDevIDRec)
{
	int j;
	RecordDescriptors* pDesc;

	printf("\t \t Record Length: %d \n", pDevIDRec->recLength);
	printf("\t \t Descriptor Count: %d \n", pDevIDRec->descCount);
	printf("\t \t Device Update Option Flags: %08x \n", pDevIDRec->devUpdateOptFlags);
	printf("\t \t Component ImageSet Version String Type: %x \n", pDevIDRec->compImgSetVerStrType);
	printf("\t \t Component ImageSet Version String Length: %d \n", pDevIDRec->compImgSetVerStrLength);
	printf("\t \t FW Device Package Data Length: %d \n", pDevIDRec->fwDevPkgDataLength);
	printf("\t \t Application Component: %x \n", pDevIDRec->appComponents);
	printf("\t \t Component ImageSet Version String: %s\n", pDevIDRec->compImgSetVerStr);

	printf("\t \t Descriptors: \n");

	pDesc = pDevIDRec->recDesc;
	for (j = 0; j < pDevIDRec->descCount; j++) {
		printf("\t \t \t Descriptor #: %d\n", j);
		PrintRecordDescriptor(pDesc);
		pDesc++;
	}
}

void PrintDownstreamDeviceIDRecord(DownstreamDeviceIDRecord* pDsDev)
{
	int j;
	RecordDescriptors* pDesc;

	printf("\t \t Downstream Record Length: %d \n", pDsDev->dsRecLength);
	printf("\t \t Downstream Descriptor Count: %d \n", pDsDev->dsDescCount);
	printf("\t \t Downstream Device Update Option Flags: %08x \n", pDsDev->dsDevUpdateOptFlags);
	printf("\t \t Downstream Dev Self Conatined Activiation Min Version String Type: %x \n", pDsDev->dsActMinVerStrType);
	printf("\t \t Downstream Dev Self Conatined Activiation Min Version String Length: %d \n", pDsDev->dsActMinVerStrLength);
	printf("\t \t Downstream Device Package Data Length: %d \n", pDsDev->dsDevPkgDataLength);
	printf("\t \t Downstream Dev Application Components: %x \n", pDsDev->dsAppComponents);
	printf("\t \t Downstream Dev Self Conatined Activiation Min Version String: %s\n", pDsDev->dsActMinVerString);
	if (pDsDev->dsDevUpdateOptFlags & BIT0) {
		printf("\t \t Downstream Dev Self Conatined Activiation Min Version Comparision Stamp: %s\n", pDsDev->dsDevActMinVerComStamp);
	}
	printf("\t \t Descriptors: \n");

	pDesc = pDsDev->dsRecDesc;
	for (j = 0; j < pDsDev->dsDescCount; j++) {
		printf("\t \t \t Descriptor #: %d\n", j);
		PrintRecordDescriptor(pDesc);
		pDesc++;
	}
}

void PrintPackageHeader(PackageHeader* pph)
{
	int i, ver;
	DeviceIDRecord* pDevIDRec; 
	DownstreamDeviceIDRecord* pDsDev;

	ComponentImageInfo* pCom;

	PrintPackageHeaderInfo(pph->hdrInfo);

	printf("\t Device ID Record Count: %d\n", pph->devIDRecCount);
	pDevIDRec = pph->devRecord;
	for (i = 0; i < pph->devIDRecCount; i++) { 
		printf("\t IDRecod: %d\n", i);
		PrintDeviceIDRecord(pDevIDRec);
		pDevIDRec++;
	}

	ver = CheckFWUpgVersion(&(pph->hdrInfo->pkgHdrId));
	if (ver == version_v11) {
		printf("\t Downstream Device ID Record Count: %d\n", pph->dsDevIDRecCount);
		pDsDev = pph->dsDevRecord;
		for (i = 0; i < pph->dsDevIDRecCount; i++) {
			printf("\t IDRecod: %d\n", i);
			PrintDownstreamDeviceIDRecord(pDsDev);
			pDsDev++;
		}
	}

	printf("\t Component Count: %d\n", pph->compCount);
	pCom = pph->compInfo;
	for (i = 0; i < pph->compCount; i++) {
		printf("\t Compnent Info: %d\n", i);
		PrintCompImageInfo(pCom);
		pCom++;
	}

	printf("\t Header Checksum: %08x \n", pph->checkSum);
}

int ConstructRecordDescriptors(unsigned char count, unsigned char* fw, RecordDescriptors* pDesc)
{
	int i, off;
	int inx = 0;
	RecordDescriptors* pD;

	for (i = 0; i < count; i++) {
		pD = (RecordDescriptors*)(fw);
		pDesc->iniDescType = pD->iniDescType;
		pDesc->iniDescLength = pD->iniDescLength;
		pDesc->iniDescData = (unsigned char*)malloc(pDesc->iniDescLength);
		off = OFFSET_OF(RecordDescriptors, iniDescData);
		inx += off;
		memcpy(pDesc->iniDescData, fw + inx, pDesc->iniDescLength);
		inx += pDesc->iniDescLength;
		pDesc++;
	}
	return inx;
}


int ConstructDevRecord(unsigned char count, unsigned char* fw, DeviceIDRecord* pDevIDRec)
{
	int k, skip, off;
	DeviceIDRecord* pDev; 
	RecordDescriptors* pDesc;
	int inx = 0;

	for (k = 0; k < count; k++) {
		pDev = (DeviceIDRecord*)(fw);
		pDevIDRec->recLength = pDev->recLength;
		pDevIDRec->descCount = pDev->descCount;
		pDevIDRec->devUpdateOptFlags = pDev->devUpdateOptFlags;
		pDevIDRec->compImgSetVerStrType = pDev->compImgSetVerStrType;
		pDevIDRec->compImgSetVerStrLength = pDev->compImgSetVerStrLength;
		pDevIDRec->fwDevPkgDataLength = pDev->fwDevPkgDataLength;
		// The size of this bitfield is based on the value contained in the ComponentBitmapBitLength
		// use 'unsigned char' here for now
		pDevIDRec->appComponents = pDev->appComponents;
		pDevIDRec->compImgSetVerStr = (unsigned char*)malloc(pDevIDRec->compImgSetVerStrLength);
		off = OFFSET_OF(DeviceIDRecord, compImgSetVerStr);
		inx += off;
		memcpy(pDevIDRec->compImgSetVerStr, fw + inx, pDevIDRec->compImgSetVerStrLength);

		inx += pDevIDRec->compImgSetVerStrLength;

		pDevIDRec->recDesc = (RecordDescriptors*)malloc(sizeof(RecordDescriptors) * pDevIDRec->descCount);
		pDesc = pDevIDRec->recDesc;

		skip = ConstructRecordDescriptors(pDevIDRec->descCount, fw + inx, pDesc);
		inx += skip;

		if (pDevIDRec->fwDevPkgDataLength) {
			pDevIDRec->fwDevPkgData = (unsigned char*)malloc(pDevIDRec->fwDevPkgDataLength);
			memcpy(pDevIDRec->fwDevPkgData, fw + inx, pDevIDRec->fwDevPkgDataLength);
			inx += pDevIDRec->fwDevPkgDataLength;
		}
		else
			pDevIDRec->fwDevPkgData = NULL;

		pDevIDRec++;
	}
	return inx;
}
int ConstructDownstreamDevRecord(unsigned char count, unsigned char* fw, DownstreamDeviceIDRecord* pDevIDRec)
{
	int k, skip, off;
	DownstreamDeviceIDRecord* pDev;
	RecordDescriptors* pDesc;
	int inx = 0;

	for (k = 0; k < count; k++) {
		pDev = (DownstreamDeviceIDRecord*)(fw);
		pDevIDRec->dsRecLength = pDev->dsRecLength;
		pDevIDRec->dsDescCount = pDev->dsDescCount;
		pDevIDRec->dsDevUpdateOptFlags = pDev->dsDevUpdateOptFlags;
		pDevIDRec->dsActMinVerStrType = pDev->dsActMinVerStrType;
		pDevIDRec->dsActMinVerStrLength = pDev->dsActMinVerStrLength;
		pDevIDRec->dsDevPkgDataLength = pDev->dsDevPkgDataLength;
		// The size of this bitfield is based on the value contained in the ComponentBitmapBitLength
		// use 'unsigned char' here for now
		pDevIDRec->dsAppComponents = pDev->dsAppComponents;
		pDevIDRec->dsActMinVerString = (unsigned char*)malloc(pDevIDRec->dsActMinVerStrLength);
		off = OFFSET_OF(DownstreamDeviceIDRecord, dsActMinVerString);
		inx += off;
		memcpy(pDevIDRec->dsActMinVerString, fw + inx, pDevIDRec->dsActMinVerStrLength);

		inx += pDevIDRec->dsActMinVerStrLength;

		if (pDevIDRec->dsDevUpdateOptFlags & BIT0) {
			pDevIDRec->dsDevActMinVerComStamp = *(unsigned int*)(fw + inx);
			inx += 4;
		}

		pDevIDRec->dsRecDesc = (RecordDescriptors*)malloc(sizeof(RecordDescriptors) * pDevIDRec->dsDescCount);
		pDesc = pDevIDRec->dsRecDesc;

		skip = ConstructRecordDescriptors(pDevIDRec->dsDescCount, fw + inx, pDesc);
		inx += skip;

		if (pDevIDRec->dsDevPkgDataLength) {
			pDevIDRec->dsDevPkgData = (unsigned char*)malloc(pDevIDRec->dsDevPkgDataLength);
			memcpy(pDevIDRec->dsDevPkgData, fw + inx, pDevIDRec->dsDevPkgDataLength);
			inx += pDevIDRec->dsDevPkgDataLength;
		}
		else
			pDevIDRec->dsDevPkgData = NULL;

		pDevIDRec++;
	}
	return inx;
}

int ConstructComponent(short int count, unsigned char* fw, ComponentImageInfo* pComp)
{
	int i,off;
	int inx = 0;
	ComponentImageInfo* pC;

		for (i = 0; i < count; i++) {
		pC = (ComponentImageInfo*)(fw+inx);
		pComp->compClassfication = pC->compClassfication;
		pComp->compIdentifier = pC->compIdentifier;
		pComp->compCompareStamp = pC->compCompareStamp;
		pComp->compOptions = pC->compOptions;
		pComp->reqCompActMethod = pC->reqCompActMethod;
		pComp->compLocOffset = pC->compLocOffset;
		pComp->compSize = pC->compSize;
		pComp->compVerStrType = pC->compVerStrType;
		pComp->compVerStrLength = pC->compVerStrLength;
		pComp->compVerString = (unsigned char*)malloc(pComp->compVerStrLength);
		off = OFFSET_OF(ComponentImageInfo, compVerString);
		inx += off;
		memcpy(pComp->compVerString, fw + inx, pComp->compVerStrLength);
		inx += pComp->compVerStrLength;

		pComp++;
	}

	return inx;
}

void FreePackageHeader(PackageHeader* pPkg)
{
	int i,k;
	DeviceIDRecord* pDev;
	DownstreamDeviceIDRecord* pDsDev;
	RecordDescriptors* pRec;
	ComponentImageInfo* pC;

	// free PackageHeaderInfo;
	if (pPkg->hdrInfo) {
		if (pPkg->hdrInfo->pkgVerString) 
			free(pPkg->hdrInfo->pkgVerString);
		free(pPkg->hdrInfo);
	}

	// free FD record
	if (pPkg->devRecord) {
		pDev = pPkg->devRecord;
		for (i = 0; i < pPkg->devIDRecCount; i++) {
			if (pDev->compImgSetVerStr) 
				free(pDev->compImgSetVerStr);
				
			// free Record descriptor
			pRec = pDev->recDesc;
			for (k = 0; k < pDev->descCount; k++) {
				if (pRec->iniDescData) 
					free(pRec->iniDescData);
				pRec++;
			}

			free(pDev->recDesc);

			if (pDev->fwDevPkgData) 
				free(pDev->fwDevPkgData);
		
			pDev++;
		}
		
		free(pPkg->devRecord);
	}

	// free Downstream record
	if (pPkg->dsDevRecord) {
		pDsDev = pPkg->dsDevRecord;
		for (i = 0; i < pPkg->dsDevIDRecCount; i++) {
			if (pDsDev->dsActMinVerString)
				free(pDsDev->dsActMinVerString);

			// free Record descriptor
			pRec = pDsDev->dsRecDesc;
			for (k = 0; k < pDsDev->dsDescCount; k++) {
				if (pRec->iniDescData)
					free(pRec->iniDescData);
				pRec++;
			}

			free(pDsDev->dsRecDesc);

			if (pDsDev->dsDevPkgData)
				free(pDsDev->dsDevPkgData);

			pDsDev++;
		}
		free(pPkg->dsDevRecord);
	}

	// free component images
	pC = pPkg->compInfo;
	for (i = 0; i < pPkg->compCount; i++) {
			if (pC->compVerString)
				free(pC->compVerString);
			pC++;
	}
	free(pPkg->compInfo);
}

int ParsePackageHeader(unsigned char* fwHeader, PackageHeader* pPkgHeader)
{
	PackageHeaderInfo* pHdrInfo;
	PackageHeaderInfo* phdr;
	DeviceIDRecord* pDevIDRec;
	DownstreamDeviceIDRecord* pDsDev;
	ComponentImageInfo* pComp;

	int off;
	int inx;
	int ver, skip;

	phdr = (PackageHeaderInfo*)fwHeader;
	pPkgHeader->hdrInfo = (PackageHeaderInfo*)malloc(sizeof(PackageHeaderInfo));
	pHdrInfo = pPkgHeader->hdrInfo;

	memcpy(pHdrInfo->pkgHdrId, phdr->pkgHdrId, 16);
	pHdrInfo->pkgHdrFmtRevision = phdr->pkgHdrFmtRevision;
	pHdrInfo->pkgHdrSize = phdr->pkgHdrSize;
	memcpy(pHdrInfo->pkgRelDateTime, phdr->pkgRelDateTime, 13);
	pHdrInfo->compBmpBitLength = phdr->compBmpBitLength;
	pHdrInfo->pkgVerStrType = phdr->pkgVerStrType;
	pHdrInfo->pkgVerStrLength = phdr->pkgVerStrLength;
	pHdrInfo->pkgVerString = (unsigned char*)malloc(phdr->pkgVerStrLength);
	off = OFFSET_OF(PackageHeaderInfo, pkgVerString);
	memcpy(pHdrInfo->pkgVerString, fwHeader + off, phdr->pkgVerStrLength);

	off = OFFSET_OF(PackageHeaderInfo, pkgVerString);
	inx = off + phdr->pkgVerStrLength;

	pPkgHeader->devIDRecCount = fwHeader[inx];
	inx++;
	
	// firmware device record
	pPkgHeader->devRecord = (DeviceIDRecord*)malloc(sizeof(DeviceIDRecord) * pPkgHeader->devIDRecCount);
	pDevIDRec = pPkgHeader->devRecord;

	skip=ConstructDevRecord(pPkgHeader->devIDRecCount, fwHeader + inx, pDevIDRec);
	inx += skip;

	// downstream device record
	ver = CheckFWUpgVersion(&pPkgHeader->hdrInfo->pkgHdrId);

	switch (ver) {
	case unknown_version:
		return E_UNKNOWN_PLDM_VERSION;
		break;
	case version_v10:
		pPkgHeader->dsDevRecord = NULL;
		break;
	case version_v11:
		pPkgHeader->dsDevIDRecCount = fwHeader[inx];
		inx++;
		pPkgHeader->dsDevRecord = (DownstreamDeviceIDRecord*)malloc(sizeof(DownstreamDeviceIDRecord) * pPkgHeader->dsDevIDRecCount);
		pDsDev = pPkgHeader->dsDevRecord;
		skip = ConstructDownstreamDevRecord(pPkgHeader->dsDevIDRecCount, fwHeader + inx, pDsDev);
		inx += skip;
		break;
	}
	
	pPkgHeader->compCount = fwHeader[inx + 1] << 8 | fwHeader[inx];
	inx += 2;

	pPkgHeader->compInfo = (ComponentImageInfo*)malloc(sizeof(ComponentImageInfo) * pPkgHeader->compCount);
	pComp = pPkgHeader->compInfo;

	skip=ConstructComponent(pPkgHeader->compCount, fwHeader + inx, pComp);
	inx += skip;
	
	pPkgHeader->checkSum = *(unsigned int*)(fwHeader + inx);
	inx += 4;
	return inx;
}
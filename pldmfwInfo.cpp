#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pldmfwinfo.h"

unsigned int crc32(unsigned char* buf, size_t size);


int main()
{
	FILE* fp;
	unsigned char fwHeader[1024];
	PackageHeader pkgHeader;
	unsigned int inx;
	unsigned int crc;

/*	// Parse the FWpackageHeader
	//fp = fopen("pldmfw.bin", "rb");
	fp = fopen("1004.fup", "rb");
	if (fp == NULL) {
		printf("File not found! \n");
		return -1;
	}

	fread(fwHeader, sizeof(unsigned char), 1024, fp);
	fclose(fp);

	inx=ParsePackageHeader(fwHeader,&pkgHeader);
	PrintPackageHeader(&pkgHeader);
	crc = crc32(fwHeader, inx-4);
	printf("Calcucated CRC32 : %08x \n", crc);
	FreePackageHeader(&pkgHeader);
	*/

	return 0;
}
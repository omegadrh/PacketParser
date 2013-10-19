// PacketParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <stdio.h>

#define MAXPACKETSIZE 9000

enum outFormat
{
	NOTSET,
	XML,
	CSV
};

bool checkArgs(int argc, char* argv[], char** inFile, outFormat &out, int &rowStart, int &rowEnd)
{
	if (argc < 2)
	{
		std::string tmpFile;
		//printf("No input file specified\n");
		std::cout << "Filename: ";
		std::getline(std::cin, tmpFile);
		*inFile = _strdup(tmpFile.c_str());
		printf("inFile is %s\n", *inFile);
		return true;
		//return false;	//needs an input file
	}
	if (argc>2)
	{
		*inFile = _strdup(argv[1]);

		for (int i = 2; i <= argc; i++)
		{
			if (strcmp(argv[i], "-x") == 0)
			{
				if (out == CSV)
				{
					printf("-x cannot be specified along with -c\n");
					return false;
				}
				else
					out = XML;
				continue;
			}
			if (strcmp(argv[i], "-c") == 0)
			{
				if (out == XML)
				{
					printf("-c cannot be specified along with -x\n");
					return false;
				}
				else
					out = CSV;
				continue;
			}
			if (strncmp(argv[i], "-r", 2) == 0)
			{
				//TODO parse beginning and ending rows
				continue;
			}
			return false;
		}
	}
	return true;
}

void printUsage(void)
{
	printf("Usage: PacketParser.exe <inputFile> [-xc] [-rX,Y]\n");
	printf("\t-x\tOutput xml\n");
	printf("\t-c\tOutput csv\n");
	printf("\t-rX,Y\tParse rows X-Y\n");
}

typedef struct packet
{
	unsigned short int field1;
	unsigned short int field2;
	unsigned short int field3;
	char *payload;
} packet;

packet * parseLine(char *line)
{
	tok = strtok(dest, " \t");
}

int parseFile(char *fileName, int startRow, int endRow)
{
	FILE *stream;
	errno_t err;
	char *dest = (char *)malloc(MAXPACKETSIZE);
	char *tok = NULL;
	packet* pak;

	err = fopen_s(&stream, fileName, "rt");
	if (err != 0)
	{
		printf("Error opening file %s\n", fileName);
		free(dest);
		return -1;
	}
	while (fgets(dest, MAXPACKETSIZE, stream))
	{
		pak = parseLine(dest);

	}
/*	err = fread_s(dest, MAXPACKETSIZE, 1, MAXPACKETSIZE, stream);
	if (err == 0)
	{
		printf("Error reading from file\n", fileName);
		free(dest);
		return -2;
	}
	*/

	fclose(stream);
	free(dest);
	return 0;
}


int _tmain(int argc, char* argv[])
{
	char *inFile = NULL;
	outFormat format = NOTSET;
	int rowStart = -1;
	int rowEnd = -1;

	if (!checkArgs(argc, argv, &inFile, format, rowStart, rowEnd))
	{
		printUsage();
		return -1;
	}

	parseFile(inFile, rowStart, rowEnd);

	return 0;
}


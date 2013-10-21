// PacketParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <deque>
#include <map>

#define MAXPACKETSIZE 9000

enum outFormat
{
	NOTSET,
	XML,
	CSV,
	TEXT
};

bool checkArgs(int argc, char* argv[], char** inFile, outFormat &out, int &rowStart, int &rowEnd)
{
	if (argc < 2)
	{
		std::string tmpFile;
		//printf("No input file specified\n");
#ifdef _DEBUG
		*inFile = _strdup("c:\\Users\\David\\Documents\\packets.txt");
		out = TEXT;
#else
		std::cout << "Filename: ";
		std::getline(std::cin, tmpFile);
		*inFile = _strdup(tmpFile.c_str());
#endif
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

typedef struct packets
{
	std::deque<packet *> packetList;
	std::multimap<unsigned short int, packet*> field1map;

} packets;

packet * parseLine(char *line)
{
	char *tok = NULL;
	char *stop = NULL;
	char *context = NULL;
	const char delim[] = " \t";
	packet * pak = new packet;

	tok = strtok_s(line, delim, &context);
	pak->field1 = (unsigned short)strtoul(tok, &stop, 16);
	
	tok = strtok_s(NULL, delim, &context);
	pak->field2 = (unsigned short)strtoul(tok, &stop, 16);

	tok = strtok_s(NULL, delim, &context);
	pak->field3 = (unsigned short)strtoul(tok, &stop, 16);

	tok = strtok_s(NULL, delim, &context);
	pak->payload = _strdup(tok);

	return pak;
}

int parseFile(char *fileName, int startRow, int endRow, packets &packetList)
{
	FILE *stream;
	errno_t err;
	char *dest = (char *)malloc(MAXPACKETSIZE);
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
		packetList.packetList.push_back(pak);
		packetList.field1map.insert(std::pair<unsigned short int, packet *>(pak->field1, pak));
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

int outputInfo(packets &packetList, outFormat format)
{
	for (std::multimap<unsigned short int, packet*>::iterator it = packetList.field1map.begin(); it != packetList.field1map.end(); ++it)
	{
		int count = packetList.field1map.count(it->first);
		switch (format)
		{
		case XML:
			break;
		case CSV:
			break;
		case TEXT:
			printf("There %s %d packet%s of type %03x\n", (count > 1 ? "are" : "is"), count, (count > 1 ? "s" : ""), it->first);
			break;
		case NOTSET:
		default:
			printf("Output format must be set");
			break;
		}
		for (int i = 1; i < count; i++)
			++it;
	}
	return 0;
}


int _tmain(int argc, char* argv[])
{
	char *inFile = NULL;
	outFormat format = NOTSET;
	int rowStart = -1;
	int rowEnd = -1;
	packets parsedList;

	if (!checkArgs(argc, argv, &inFile, format, rowStart, rowEnd))
	{
		printUsage();
		return -1;
	}

	parseFile(inFile, rowStart, rowEnd, parsedList);

	outputInfo(parsedList, format);

	return 0;
}


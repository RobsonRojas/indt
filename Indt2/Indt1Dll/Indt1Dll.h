#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <memory>

using namespace std;

#ifdef _WIN32
#    ifdef LIBRARY_EXPORTS
#        define LIBRARY_API __declspec(dllexport)
#    else
#        define LIBRARY_API __declspec(dllimport)
#    endif
#elif
#    define LIBRARY_API
#endif

#define HEADER_BEGIN 0X47
#define HEADER_END 0XDD
#define BODY_BEGIN 0X48
#define BODY_END 0Xffffffaa
#define FILE_END_BYTE 0Xffffffff

#define APTTABLE 0xA 
#define MPTTABLE 0xB 
#define ITNTABLE 0xC 
#define ATBTABLE 0xD 
#define TBATABLE 0xE 
#define TAPTABLE 0xF 

enum States
{
	ST_WAIT_BEGIN,
	ST_HEADER_BODY_BYTE1,
	ST_HEADER_BODY_BYTE2,
	ST_HEADER_END,
	ST_TB_BEGIN,
	ST_TB_TABLE_ID,
	ST_TB_TABLE_APT_NUM_OF_PROGRAMS,
	ST_TB_TABLE_APT_NETWORK_NAME,
	ST_TB_TABLE_MPT_PROGRAM_NAME,
	ST_TB_TABLE_MPT_PROGRAM_TYPE,
	ST_TB_TABLE_TBA_SERVICE_NUMBER,
	ST_TB_TABLE_TBA_NETWORK_NUMBER,
	ST_TB_BODY,
	ST_TB_END,
	ST_FILE_SEC_END_BYTE
};


struct Table {
	unsigned int table_id;
	unsigned int table_size;
};

struct APT_Table : public Table {
	unsigned int num_of_programs;
	unsigned int network_name;
};

struct MPT_Table : public Table {
	unsigned int program_name;
	unsigned int program_type;
};

struct TBA_Table : public Table {
	unsigned int service_number;
	unsigned int network_number;
};

struct File_Parse {
	unsigned int byte1;
	unsigned int byte2;
	unsigned int APT_table_count;
	unsigned int MPT_table_count;
	unsigned int ITN_table_count;
	unsigned int ATB_table_count;
	unsigned int TBA_table_count;
	unsigned int TAP_table_count;

	list<shared_ptr<Table> > tables;
};


LIBRARY_API void ParseFile(const char*  file, File_Parse &file_parse);
LIBRARY_API void PrintFileParse(File_Parse &file_parse);
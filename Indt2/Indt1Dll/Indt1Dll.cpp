// Indt1Dll.cpp : Defines the exported functions for the DLL application.
//

#include "Indt1Dll.h"

void ParseFile(const char*  file, File_Parse &file_parse)
{
	unsigned int count_tb_bytes = 0;
	unsigned char table_id = 0;
	unsigned char num_of_programs = 0;
	unsigned char network_name = 0;
	unsigned char program_name = 0;
	unsigned char program_type = 0;
	unsigned char service_number = 0;
	unsigned char network_number = 0;

	shared_ptr<Table> tb;
	list<shared_ptr<Table>>::iterator it;

	std::ifstream source(file, ios::in | ios::binary);

	char byte = 0;
	States state = ST_WAIT_BEGIN;
	bool end_of_file = false;
	while (source.get(byte) && !end_of_file)
	{
		switch (state)
		{
		case ST_WAIT_BEGIN:
			if (byte == HEADER_BEGIN) {
				state = ST_HEADER_BODY_BYTE1;
			}
			break;
		case ST_HEADER_BODY_BYTE1:
			/* process header body byte */
			file_parse.byte1 = byte;
			state = ST_HEADER_BODY_BYTE2;
			break;
		case ST_HEADER_BODY_BYTE2:
			/* process header body byte */
			file_parse.byte2 = byte;
			state = ST_HEADER_END;
			break;
		case ST_HEADER_END:
			/* process header end byte */
			state = ST_TB_BEGIN;
			break;
		case ST_TB_BEGIN:
			/* process tb body begin or end file */
			if (byte == FILE_END_BYTE) {
				state = ST_FILE_SEC_END_BYTE;
			}
			else {
				state = ST_TB_TABLE_ID;
			}
			break;
		case ST_TB_TABLE_ID:
			count_tb_bytes++;
			table_id = byte;

			if (byte == APTTABLE) {
				state = ST_TB_TABLE_APT_NUM_OF_PROGRAMS;
			}
			else if (byte == MPTTABLE) {
				state = ST_TB_TABLE_MPT_PROGRAM_NAME;
			}
			else if (byte == TBATABLE) {
				state = ST_TB_TABLE_TBA_SERVICE_NUMBER;
			}
			else {
				state = ST_TB_BODY;
			}
			break;
		case ST_TB_BODY:
			count_tb_bytes++;
			if (byte == BODY_END)
			{
				state = ST_TB_END;
			}
			break;
		case ST_TB_TABLE_APT_NUM_OF_PROGRAMS:
			count_tb_bytes++;
			num_of_programs = byte;
			state = ST_TB_TABLE_APT_NETWORK_NAME;
			break;
		case ST_TB_TABLE_APT_NETWORK_NAME:
			count_tb_bytes++;
			network_name = byte;
			state = ST_TB_BODY;
			break;
		case ST_TB_TABLE_MPT_PROGRAM_NAME:
			count_tb_bytes++;
			program_name = byte;
			state = ST_TB_TABLE_MPT_PROGRAM_TYPE;
			break;
		case ST_TB_TABLE_MPT_PROGRAM_TYPE:
			count_tb_bytes++;
			program_type = byte;
			state = ST_TB_BODY;
			break;
		case ST_TB_TABLE_TBA_SERVICE_NUMBER:
			count_tb_bytes++;
			service_number = byte;
			state = ST_TB_TABLE_TBA_NETWORK_NUMBER;
			break;
		case ST_TB_TABLE_TBA_NETWORK_NUMBER:
			count_tb_bytes++;
			network_number = byte;
			state = ST_TB_BODY;
			break;
		case ST_TB_END:
			tb = make_shared<APT_Table>();
			if (table_id == APTTABLE) {
				shared_ptr<APT_Table> apt_tb = make_shared<APT_Table>();
				apt_tb->num_of_programs = num_of_programs;
				apt_tb->network_name = network_name;
				tb = apt_tb;
			}
			else if (table_id == MPTTABLE) {
				shared_ptr<MPT_Table> apt_tb = make_shared<MPT_Table>();
				apt_tb->program_name = program_name;
				apt_tb->program_type = program_type;
				tb = apt_tb;
			}
			else if (table_id == TBATABLE) {
				shared_ptr<TBA_Table> apt_tb = make_shared<TBA_Table>();
				apt_tb->service_number = service_number;
				apt_tb->network_number = network_number;
				tb = apt_tb;
			}
			else {
				tb = make_shared<Table>();
			}

			tb->table_id = table_id;
			tb->table_size = count_tb_bytes;

			if (table_id == APTTABLE) {
				file_parse.APT_table_count++;
			}
			if (table_id == MPTTABLE) {
				file_parse.MPT_table_count++;
			}
			if (table_id == ITNTABLE) {
				file_parse.ITN_table_count++;
			}
			if (table_id == ATBTABLE) {
				file_parse.ATB_table_count++;
			}
			if (table_id == TBATABLE) {
				file_parse.TBA_table_count++;
			}
			if (table_id == TAPTABLE) {
				file_parse.TAP_table_count++;
			}
			// tries find other table with the same table id
			it = file_parse.tables.begin();
			for (; it != file_parse.tables.end(); ++it) {
				if ((*it)->table_id == table_id)
				{
					break;
				}
			}

			if (it != file_parse.tables.end())
			{
				// insert after - group
				//std::cout << "insert" << std::endl;
				file_parse.tables.insert(++it, tb);
			}
			else {
				//new table
				//std::cout << "insert end" << std::endl;
				file_parse.tables.push_back(tb);
			}

			if (byte == FILE_END_BYTE) {
				state = ST_FILE_SEC_END_BYTE;
			}
			else {
				state = ST_TB_TABLE_ID;
			}
			break;
		case ST_FILE_SEC_END_BYTE:
			/* process file end */
			if (byte == FILE_END_BYTE) {
				end_of_file = true;
			}
			break;

		default:
			break;
		}
	}
	source.close();
}

void PrintFileParse(File_Parse &file_parse)
{
	list<shared_ptr<Table>>::iterator it;

	std::cout << std::endl << "############## File Header ############" << std::endl;
	std::cout << "byte1:" << std::hex << file_parse.byte1 << std::endl;
	std::cout << "byte2:" << std::hex << file_parse.byte2 << std::endl;

	std::cout << std::endl << "############## Begin Tables ############" << std::endl;
	std::cout << "number of APT tables:" << file_parse.APT_table_count << std::endl;
	std::cout << "number of MPT tables:" << file_parse.MPT_table_count << std::endl;
	std::cout << "number of ITN tables:" << file_parse.ITN_table_count << std::endl;
	std::cout << "number of ATB tables:" << file_parse.ATB_table_count << std::endl;
	std::cout << "number of TBA tables:" << file_parse.TBA_table_count << std::endl;
	std::cout << "number of TAP tables:" << file_parse.TAP_table_count << std::endl;

	it = file_parse.tables.begin();
	for (; it != file_parse.tables.end(); ++it) {
		shared_ptr<Table> table = (*it);
		std::cout << std::endl << "Table:" << std::endl;
		std::cout << "table id:" << table->table_id << std::endl;
		std::cout << "table size:" << table->table_size << std::endl;

		if (table->table_id == APTTABLE) {
			std::cout << std::endl << "Apt table data:" << std::endl;
			shared_ptr<APT_Table> apt_tb = static_pointer_cast<APT_Table>(table);
			std::cout << "num of programs:" << std::hex << apt_tb->num_of_programs << std::endl;
			std::cout << "network name:" << std::hex << apt_tb->network_name << std::endl;
		}
		else if (table->table_id == MPTTABLE) {
			std::cout << std::endl << "MPT table data:" << std::endl;
			shared_ptr<MPT_Table> mpt_tb = static_pointer_cast<MPT_Table>(table);
			std::cout << "program name:" << std::hex << mpt_tb->program_name << std::endl;
			std::cout << "program type:" << std::hex << mpt_tb->program_type << std::endl;
		}
		else if (table->table_id == TBATABLE) {
			std::cout << std::endl << "TBA table data:" << std::endl;
			shared_ptr<TBA_Table> tba_tb = static_pointer_cast<TBA_Table>(table);
			std::cout << "service number:" << std::hex << tba_tb->service_number << std::endl;
			std::cout << "network number:" << std::hex << tba_tb->network_number << std::endl;
		}
	}
	std::cout << std::endl << "############## End Tables ############" << std::endl;

}

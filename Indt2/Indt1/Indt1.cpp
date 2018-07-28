// Indt1.cpp : Defines the entry point for the console application.
//
#include "Indt1Dll.h"


int main() {
	File_Parse file_parse = {0};
	const char file[] = "teste.bin";

	ParseFile(file, file_parse);
	PrintFileParse(file_parse);
	getchar();
	return 0;
}

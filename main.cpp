/*
 * main.cpp
 *
 * Hlavny programovy subor, robi setky take veci okolo parametrov, fajlov a tak...
 */

#include <cstdio>
#include <string>
#include <cstdlib>
#include "globals.h"
#include "parse.h"
#include "execute.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {		// dakto asi nechape...
		printf("USAGE:\n");
		printf("\tofca <input_file_name>\n");
		exit(1);
	}
	parse_file(argv[1]);		// parsneme argument
	size_t line_number;
	db = new dbase;		// alokujeme space pre premenne
	string result = execute(line_number); 	// vykoname program
	delete db;		// uvolnime pamat premennych
	deallocate_nodes();		// uvolnenie pamate
	if (result != "") {		// chybova hlaska je neprazdna
		printf("CHYBA!\n%s\nSubor: %s, Riadok: %d\n", result.data(), argv[1], line_number);
		exit(1);
	}
	return 0;
}

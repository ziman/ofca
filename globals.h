/*
 * globals.h
 *
 * V tomto subore su vsetky globalne premenne plus nejake utilitky
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include "dbase.h"

using namespace std;

extern dbase *db;	// databaza premennych

void lowcase(string &s);	// skonvertuje string na male pismena
string inttostr(int x);		// konvertuje int na string
string readln(FILE *f);		// precita riadok zo suboru a vrati ho v stringu
void crypt(string &s);		// zakryptuje string uzasnym algoritmom :)
bool valid_number(string x);	// zisti, ci x sa da zmysluplne konvertovat na cislo

#endif

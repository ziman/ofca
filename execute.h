#ifndef EXECUTE_H
#define EXECUTE_H

#include "parse.h"
#include <cstdio>
#include <string>

using namespace std;

/*
 * Tato procedura robi samotne vykonavanie programu
 * v jazyku OFCA. Berie jeden parameter (referenciu),
 * do ktoreho ulozi cislo riadku v pripade chyby.
 *
 * Pri chybe vrati jej opis, ak chyba nenastane,
 * vrati prazdny retazec.
 */
string execute(size_t &line_number);

#endif

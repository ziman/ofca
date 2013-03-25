#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>

#define NAME_SIZE 256

using namespace std;

enum EType {etString, etOp};	// typ elementu

struct Element {
	EType type;	// typ
	string *name;	// meno == obsah
	string *fname;	// meno zdrojoveho suboru
	size_t lnumber;	// cislo riadka v *.ofca subore, pouziva sa pri hlaseni chyb
};
	
Element *create_element(void);	// alokuje pamat potrebnu pre jeden element
void delete_element(Element *root);	// dealokuje element
void deallocate_nodes(void);	// dealokuje vsetky elementy vo vektore nodes

/*
 * parse()
 * 
 * procedura, ktora rozbije string line na elementy,
 * ktorym priradi cislo riadka a ulozi do vektora nodes
 *
 * pri vyskyte chyby vrati retazec s jej opisom, ak sa
 * chyba nevyskytne, vrati prazdny retazec
 */
string parse(string line, size_t line_number, const string filename);	

/*
 * parse_file()
 *
 * procedura, ktora zaradi do vektora nodes elementy
 * zo zadaneho suboru so slohom v jazyku OFCA
 *
 * o navratovej hodnote plati vyssie uvedene.
 */
string parse_file(string filename);

extern vector<Element *> nodes;	// globalny zoznam elementov

#endif

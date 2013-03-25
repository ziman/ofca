#include "parse.h"
#include "globals.h"
#ifdef DEBUG
	#include <cstdio>
#endif

vector<Element *> nodes;	// samotna definicia nodov

bool isblank(char ch)
{
	return ((ch == ' ') || (ch == '\n') || (ch == '\t') || (ch == '\r'));
}

string parse_file(const string filename)
{
	FILE *f = fopen(filename.data(), "r");
	if (f == NULL) return "Nemozem otvorit subor!";

	string line;	// nacitame naraz jeden riadok
	string result;	// tu si budeme ukladat navratovu hodnotu
	size_t line_number = 1;	// pocitadlo riadkov, hodi sa pri debugovani :)
	while (!feof(f)) {
		line = readln(f);
		result = parse(line, line_number, filename);	// rozbijeme riadok na elementy
		if (result != "") {	// nastala chyba - zahlasime a skoncime
			result += " (";
			result += filename + ", riadok c.";
			result += inttostr(line_number) + ")";
			return result;
		};
		line_number++;
	}
	fclose(f);	// :)
	return "";
}

string parse(string line, size_t line_number, const string filename)
{
	size_t a,b;	// a - index prveho pismena elementu, b - index prveho pismenka za elementom 
	a = 0;
	while (a<line.size()) {	// az do konca lajny
		while ( (a<line.size()) && (isblank(line[a]))) a++;	// preskocime prazdne
		if (isblank(line[a])) return "";	// lajna bola cela prazdna, to nie je chyba
		b = a;
		Element *node = create_element();
		node->lnumber = line_number;	// nastavime elementu cislo riadka
		*(node->fname) = filename;		// aj meno suboru
		if (line[a] == '(') {	// toto bude komentar
			b++;
			while ((b<line.size()) && (line[b] != ')')) b++;	// hladame druhu zatvorku
			if (line[b] != ')') return "Neukonceny komentar!";
			node->type = etOp;
			*(node->name) = "";
		} else
		if (line[a] == '\"') {	// ak sa element zacina uvodzovkou, bude to retazec
			b++;
			do {
				while ((b<line.size()) && (line[b] != '\"')) b++;	// hladame druhu uvodzovku
			} while ((b<line.size()) && (line[b-1] == '\\'));	// ale ne taku, co ma pred sebou backslash (\")
			if ((line[b] != '\"') || (line[b-1] == '\\')) return "Neukoncena veta!";
			node->type = etString;
			a++;
			node->name->assign(line, a, b-a);	// vystrihneme si element z lajny
			string &s = *(node->name);
			size_t i = 0;
			while (i<s.size()-1) {			// specialne znaky
				if (s[i] == '\\') {
					switch (s[i+1]) {
						case 'n' :
							s[i] = '\n';
							break;
						case 't' :
							s[i] = '\t';
							break;
						case 'r' :
							s[i] = '\r';
							break;
						case '\\':
							s[i] = '\\';
							break;
					}
					s.erase(i+1, 1);
				}
				i++;
			}
		} else {	// nie je to retazec
			while ( (b<line.size()) && (!isblank(line[b]))) b++;	// preskakujeme medzery
			node->type = etOp;			// je to operator
			node->name->assign(line, a, b-a);	// vystrihneme si obsah z lajny
			string &s = *(node->name);		// urobim si alias, aby som nepisal jak idiot
			lowcase(s);				// vsetko automaticky na male pismena
			for (size_t i=0; i<s.size(); i++) {
				if (s[i] == 'y') s[i] = 'i';	// zlikvidujeme ypsilon
				
				if (s[i] == ',') s.erase(i--,1);	// aj ostatne znaky :)
				if (s[i] == '.') s.erase(i--,1);
				if (s[i] == ';') s.erase(i--,1);
				if (s[i] == ':') s.erase(i--,1);
				if (s[i] == '!') s.erase(i--,1);
			}
		};
		if ((node->type == etOp) && 	// dokreslovacie slovicka vyhodime
				((*(node->name) == "toten") || (*(node->name) == "ten") ||
				 (*(node->name) == "toto")  || (*(node->name) == "to")  ||
				 (*(node->name) == "tota")  || (*(node->name) == "ta")  ||
				 (*(node->name) == ")")    || (*(node->name) == "asik" ) ||
				 (*(node->name) == "") 						)) {
			// nepushneme to z logickych dovodov :)
			delete_element(node);
		} else {
			nodes.push_back(node); 	// ale tu hej
		}
		a = b + 1;	// ideme na dalsie slovo
	}
	return "";	// vsetko skoncilo normalne, bez chyb
}

// zvysok azda hovori za seba...

Element *create_element(void)
{
	Element *result = new Element();
	result->name = new string;
	result->fname = new string;
	return result;
}

void delete_element(Element *root)
{
	delete root->name;
	delete root->fname;
	delete root;
}

void deallocate_nodes(void)
{
	size_t n = nodes.size();
	for (size_t i=0; i<n; i++)
		delete_element(nodes[i]);
	nodes.resize(0);
}

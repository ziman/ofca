#include "execute.h"
#include "globals.h"
#include "dbase.h"
#include <cstdio>
#include <stack>
#include <cstdlib>

dbase *db;

bool pravda(string x) 	// skonvertuje string na bool
{
	return valid_number(x) && (atoi(x.data()) != 0);
}

/*
 * UPOZORNENIE:
 * Kedze sa vsetky identifiktory (etOp) okamzite kovertuju na male pismena
 * a y->i, vsetky treba pisat s makkym i !!!
 */

/*
 * evaluate()
 *
 * vyhodnoti vyraz zacinajuci v poli nodes prvkom nodes[i]
 * a vrati vysledok
 *
 * ak nastane chyba, nastavi error na true a namiesto vysledku
 * vrati opis chyby
 */
string evaluate(size_t &i, bool &error)
{
	error = false;
	string &value = *(nodes[i]->name);
	if ((nodes[i]->type == etString) || (isdigit(value[0]))) {
		return value;
	} else {	// priradenie <premenna> = <premenna>
		if (value == "viraz") {	// aritmeticky vyraz
			i++;
			string lvalue = evaluate(i, error);	// lavy operand; rekurzivne
			if (error) return lvalue;
			i++;
			string &op = *(nodes[i]->name);		// operator
			i++;
			string rvalue = evaluate(i, error);	// pravy operand
			if (error) return rvalue;
			if (op == "plus") {
				if (valid_number(lvalue) && valid_number(rvalue))
					return inttostr(atoi(lvalue.data()) + atoi(rvalue.data()));
				else
					return lvalue + rvalue;
			} else
			if (op == "minus") {
				if (valid_number(lvalue) && valid_number(rvalue))
					return inttostr(atoi(lvalue.data()) - atoi(rvalue.data()));
				else {
					error = true;
					return "Minus sa da pouzit iba na cisla!";
				}
			} else
			if (op == "krat") {
				if (valid_number(lvalue) && valid_number(rvalue))
					return inttostr(atoi(lvalue.data()) * atoi(rvalue.data()));
				else {
					error = true;
					return "Nasobenie sa da pouzit iba na cisla!";
				}
			} else
			if ((op == "je") || (op == "rovnasa") || (op == "rovne") || (op == "rovny")) {
				if (lvalue == rvalue)
					return "1";
				else
					return "0";
			} else
			if ((op == "vacsie") || (op == "vacsi")) {
				if (atoi(lvalue.data()) > atoi(rvalue.data()))
					return "1";
				else
					return "0";
			} else
			if ((op == "mensie") || (op == "mensi")) {
				if (atoi(lvalue.data()) < atoi(rvalue.data()))
					return "1";
				else
					return "0";
			} else
			if ((op == "nevacsie") || (op == "nevacsi")) {
				if (atoi(lvalue.data()) <= atoi(rvalue.data()))
					return "1";
				else
					return "0";
			} else
			if ((op == "nemensie") || (op == "nemensi")) {
				if (atoi(lvalue.data()) == atoi(rvalue.data()))
					return "1";
				else
					return "0";
			} else
			if (op == "a") {
				if (pravda(lvalue) && pravda(rvalue))
					return "1";
				else
					return "0";
			} else
			if (op == "alebo") {
				if (pravda(lvalue) || pravda(rvalue))
					return "1";
				else
					return "0";
			} else
			if ((op == "lomene") || (op == "delene")) {
				if (valid_number(lvalue) && valid_number(rvalue)) {
					if (atoi(rvalue.data()) == 0) {
						error = true;
						return "Delenie nulou!";
					} else
						return inttostr(atoi(lvalue.data()) / atoi(rvalue.data()));
				} else {
					error = true;
					return "Delenie sa da pouzit iba na cisla!";
				}
			} else {
				error = true;
				return op + "? Taky operator nepoznam!";
			}
		} else {	// obycajne priradenie
			return db->read(value);
		}
	}
}

string execute(size_t &line_number)
{
	stack<size_t> jmp;	// zasobnik skokov - pouziva sa na cykly a volanie procedur
	stack<dbase*> vars;	// zasobnik databaz lokalnych premennych pri volani f(x)
	dbase proc;		// databaza procedur -> uchovava mena procedur a ich entry pointy
	bool error = false;	// pomocna premenna na errory
	bool skipelse = false;	// preskocit najblizsi else?
	size_t i = 0;		// index elementu, s ktorym sa pracuje
	while (i<nodes.size()) {	// az do konca
		string name = *(nodes[i]->name);	// referencia na meno elementu, zeby som nepisal jak taky idiot
		line_number = nodes[i]->lnumber;	// do line_number ulozime cislo riadku, keby nahodou vznikla chyba
		if (name == "a") {	// tento prikaz sa preskakuje
		} else
		if (name == "zrob") {	// volanie procedury
			if (i == nodes.size()-1)
				return "Co mam zrobic?";
			i++;
			string &arg = *(nodes[i]->name);
			size_t n = proc.read(arg, 0);	// zistime si, kam mame skocit
			if (n != 0) {		// ak je to platna adresa
				jmp.push(i+1);	// pushneme aktualnu adresu
				vars.push(db);	// pushneme aktualne premenne
				db = new dbase;
				*db = *vars.top();
				i = n;		// skocime tam
				continue;
			} else return "Si nenapisal este coska...";
		} else
		if (name == "coska") {	// definicia procedury
			if (i == nodes.size()-1)
				return "Jak sa ma coska volac?";
			i++;
			proc.write(*(nodes[i]->name), i+1);	// zapiseme si miesto, kam treba skocit
			i++;
			while ((i<nodes.size()-1) && ((*(nodes[i]->name) != "konec") || (*(nodes[i+1]->name) != "cehoska"))) i++;
			if (((*(nodes[i]->name) != "konec") || (*(nodes[i+1]->name) != "cehoska")))
				return "Coska si neukoncil!";
			i++;
		} else
		if (name == "enter") {
			printf("\n");
		} else
		if (name == "napis") {
			if ((i == nodes.size()-1))
				return "Si nezadal, co treba napisat!";
			i++;
			string s = evaluate(i, error);
			if (error) return s;
			printf("%s", s.data());
		} else
		if (name == "ked") {	// ked ne = else
			if ((i == nodes.size()-1) || (*(nodes[i+1]->name) != "ne"))	// pravidla sa musia dodrzat :)
				return "Za \"ked\" chyba \"ne\"";
			i+=2;
			if (skipelse) {		// ak bola predchadzajuca podmienka splnena, musime else preskocit
				size_t depth = 1;	// hlba ifa
				while ((i<nodes.size()) && (depth>0)) {
					i++;
					if (*(nodes[i]->name) == "ak")	// hlbka++
						depth++;
					else if ((*(nodes[i]->name) == "konec") && (*(nodes[i+1]->name) == "ifa")) {	// hlbka--
					i++;
						depth--;
					}
				}
				if (depth > 0)	// evidentna chyba vyvojara v OFCI
					return "Tha ti dajak usli konce ifoch!";
			}
		} else
		if (name == "ak") {	// podmienka
			i++;
			bool error;
			string x = evaluate(i, error);	// vyhodnotime nasledujuci vyraz
			if (error) return x;
			i++;
			if (!pravda(x)) {	// ak je nepravdivy, preskocime blok
				size_t depth = 1;
				while ((i<nodes.size()) && (depth>0)) {
					i++;
					if (*(nodes[i]->name) == "ak")
						depth++;
					else if (((*(nodes[i]->name) == "konec") && (*(nodes[i+1]->name) == "ifa")) ||
						 ((depth == 1) && ((*nodes[i]->name == "ked") && (*(nodes[i+1]->name) == "ne")))) {
						i+=2;
						depth--;
					}
				}
				skipelse = false;	// ak je nepravdivy, else blok nebudeme preskakovat
				if (depth > 0)
					return "Tha ti dajak usli konce ifoch!";
			} else {
				skipelse = true;	// ale ak je pravdivy, najblizsie else musime preskocit
			}
			continue;
		} else
		if (name == "popitaj") {	// nacita od uzivatela vstup
			if ((i == nodes.size()-1))
				return "Si nezadal, co treba napisat!";
			i++;
			string &s = *(nodes[i]->name);
			db->write(s, readln(stdin));
		} else
		if (name == "zvacsi") {		// inkrementacia
			if ((i == nodes.size()-1))
				return "Nenije co zvacsit!";
			i++;
			if (nodes[i]->type != etOp)
				return "Nemozes zvacsovat retazce! Si saleny?!";
			string &s = *(nodes[i]->name);
			db->write(s, inttostr(atoi(db->read(s).data()) + 1));
		} else
		if (name == "zmensi") {		// dekrementacia
			if ((i == nodes.size()-1))
				return "Nenije co zmensit!";
			i++;
			if (nodes[i]->type != etOp)
				return "Nemozes zmensovat retazce! Si saleny?!";	// :)
			string &s = *(nodes[i]->name);
			db->write(s, inttostr(atoi(db->read(s).data()) - 1));
		} else
		if (name == "kolecko") {	// cyklus
			jmp.push(i);		// pushneme aktualnu poziciu
		} else 
		if (name == "nazad") {		// ekvivalent prikazu 'continue'
			i = jmp.top();		// zistime, kam sa mame dostat
			jmp.pop();		// a odstranime hodnotu z vrchu zasobnika
			continue;
		} else
		if (name == "ubzikni") {	// ekvivalent prikazu 'break'
			jmp.pop();		// zahodime navratovu adresu
			while ((i<nodes.size()) && ((*(nodes[i-1]->name) != "konec") || (*(nodes[i]->name) != "kolecka")))
				i++;		// a hladame konec kolecka
		} else 
		if (name == "konec") {
			if ((i == nodes.size()-1) || (nodes[i+1]->type !=etOp))
				return "Neviem, co sa konci!";
			string &arg = *(nodes[i+1]->name);
			i++;
			if ((arg == "sveta") || (arg == "programa") || (arg == "ofce")) {	// toto je azda jasne
				return "";	// ukoncime bez chybovej hlasky
			} else
			if (arg == "kolecka") {	// konec kolecka
				i = jmp.top();	// zistime, kam sa mame vratit
				jmp.pop();	// odstranime navratovu hodnotu zo zasobnika
				continue;
			} else
			if (arg == "ifa") {	// tu nebudeme robit nic, konec ifa sa pouziva iba pri preskakovani ifov
			} else
			if (arg == "cehoska") {	// konec cehoska
				i = jmp.top();	// popneme navratovu adresu
				jmp.pop();
				string vysledok = db->read("visledok");	// zapamatame si vysledok
				delete db;		// zmazeme databazu
				db = vars.top();	// vratime dbazu premennych do povodneho stavu
				vars.pop();
				db->write("visledok", vysledok);		// vysledok ulozime do povodnej databazy
				continue;
			}
		} else {	// povazujeme vyraz za premennu, ked to neni prikaz, alebo ked je uvedeny slovom menak
			do {
				if (name == "menak") {
					i++;	// je to menak
					name = *(nodes[i]->name);
				}
				if ((i == nodes.size()-1) || (nodes[i+1]->type != etOp))	// nenasleduje operator?!
					return name + ": Neviem, co mam robit s touto premennou";
				string &arg = *(nodes[i+1]->name);
				i++;
				if ((arg == "je") || (arg == "bude")) {	// priradenie
					if ((i == nodes.size()-1))
						return "A ty si myslis, ze ja si len tak z prsta vycucnem, co treba priradit?!";
					i++;
					string mod = *(nodes[i]->name);
					if (mod == "furt") {	// furt == konstanta, zatial sa len preskakuje... (nema vplyv)
						if ((i == nodes.size()-1))
							return "A ty si myslis, ze ja si len tak z prsta vycucnem, jak to furt bude?!";
						i++;
					}
					string s = evaluate(i, error);	// vyhodnotime vyraz na pravej strane
					if (error) return s;		// pri chybe ukoncime s chybovou hlaskou
					db->write(name, s);		// inac zapiseme do premennej danu hodnotu
				} else 
					return "Co ine sa s premennou da robit ako priradit do nej daco?!";
				i++;
			} while (*(nodes[i]->name) == "a");	// spojky a preskakujeme
			continue;
		}
		i++; // skok na nasledujuci riadok
	}
	return "";	// korektne ukoncenie, bez chybovej hlasky
}

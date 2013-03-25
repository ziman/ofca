/*
 * dbase.h
 *
 * Tento modul sa stara o premenne.
 * Ma formu asociativneho objektu, v ktorom sa ukladaju pary
 * 	kluc -> hodnota
 * Podla kluca sa potom v databaze vyhladava
 * Kluce su case-insensitive
 * Vzhladom na sposob ukladania klucov do suboru, pokial planujes
 * ukladat databazu do suboru, nesmies v entries pouzivat \n-ka,
 * ak vsak iba ostanu v pamati bez ukladania do suboru, na entry
 * values sa nekladu ziadne obmedzenia (to plati aj o ich dlzke).
 */

#ifndef DBASE_H
#define DBASE_H

#include <string>
#include <cstdio>
#include <vector>

using namespace std;

struct var {
	string *name, *value;
};

class dbase {
	private:
		vector<var> db;
		string fn;
	protected:
	public:
		dbase(string filename = "");	// konstruktor, volitelny nazov suboru, ak je specifikovany, hned sa loadne
		~dbase(void);	// destruktor. ak bol konstruktoru urceny nazov suboru, automaticky sa don dBaza ulozi
		string read(string key);
		int read(string key, int default_value);
		void write(string key, const string value);
		void write(string key, int value);
		void clear(void);
		bool load(string filename);	// vrati false pri neuspechu
		bool save(string filename);	// vrati false pri neuspechu
		dbase &operator=(dbase& x);	// kopirovaci operator =
};

#endif

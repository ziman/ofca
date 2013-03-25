#include "dbase.h"
#include "globals.h"
#include <iterator>
#include <cstdlib>

#define BUF_SIZE 1024

using namespace std;

dbase::dbase(string filename)
{
	fn = filename;
	if (fn != "") load(fn);
}

dbase::~dbase(void)
{
	if (fn != "") save(fn);
}

dbase &dbase::operator=(dbase &x)
{
	clear();
	size_t i = 0, n = x.db.size();
	while (i < n) {
		write(*(x.db[i].name), *(x.db[i].value));
		i++;
	}
	return *this;
}

string dbase::read(string key)
{
	lowcase(key);
	size_t i = 0;
	size_t n = db.size();
	while (i < n) {
		if (*(db[i].name) == key) return (*(db[i].value));
		i++;
	}
	return "";
}

void dbase::write(string key, const string value)
{
	lowcase(key);
	size_t i = 0;
	size_t n = db.size();
	while (i < n) {
		if (*(db[i].name) == key) {
			*(db[i].value) = value;
			return;
		}
		i++;
	}
	var x;
	x.name = new string(key);
	x.value = new string(value);
	db.push_back(x);
}

void dbase::clear(void)
{
	size_t i = 0, n = db.size();
	while (i < n) {
		delete db[i].name;
		delete db[i].value;
		i++;
	}
	db.erase(db.begin(), db.end());
}

int dbase::read(string key, int default_value)
{
	string tmp = read(key);
	if (!valid_number(tmp)) return default_value;
	return atoi(tmp.data());
}

void dbase::write(string key, int value)
{
	write(key, inttostr(value));
}

bool dbase::save(string filename)
{
	FILE *f = fopen(filename.data(), "w");
	if (f == NULL) return false;
	size_t i = 0, n = db.size();
	while (i < n) {
		if (*(db[i].value) != "")
			fprintf(f, "%s\n%s\n", db[i].name->data(), db[i].value->data());
		i++;
	}
	return true;
	fclose(f);
}

bool dbase::load(string filename)
{
	FILE *f = fopen(filename.data(), "r");
	if (f == NULL) return false;
	clear();
	var x;
	while (!feof(f)) {
		x.name = new string(readln(f));
		x.value = new string(readln(f));
		db.push_back(x);
	}
	fclose(f);
	return true;
}

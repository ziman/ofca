#include "globals.h"
#include <cstdlib>

#define BUF_SIZE 1024

string readln(FILE *f)
{
	char buf[BUF_SIZE];
	fgets(buf, BUF_SIZE, f);	// precitame buffer
	for (int i=0; i<BUF_SIZE; i++)
		if (buf[i] == '\n') {	// musime vyhodit vsetky \n-ka
			buf[i] = '\0';
			break;
		}
	return buf;
}

void lowcase(string &s)
{
	for (size_t i=0; i<s.size(); i++)
		if ((s[i]>='A') && (s[i]<='Z'))
			s[i] = s[i] + 'a'-'A';
}

string inttostr(int x)
{
	string result;
	char buf[2];
	buf[1] = '\0';
	while (x > 0) {
		*buf = '0' + (x%10);
		result.insert(0, buf);
		x = x/10;
	}
	if (result == "") result = "0";
	return result;
}

bool valid_number(string x)
{
	if (x.size() == 0) return false;
	if (x[0] == '-') x.erase(0,1);
	for (size_t i=0; i<x.size(); i++)
		if (!isdigit(x[i])) return false;
	return true;
}

void crypt(string &s)
{
	unsigned int sum = 0;
	for (size_t i=0; i<s.size(); i++)
		sum += s[i];
	srand(sum);				// nastavime randSeed :)
	for (size_t i=0; i<s.size(); i++)
		s[i] = 'A' + (s[i] + rand()) % 64;
}

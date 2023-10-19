#ifndef GROCER_H
#define GROCER_H
#include <string>
#include <map>
#include <fstream>
using namespace std;

void backup();
int parseInt(const string& s);

class Menu {
private:
	void printMenu();

public:
	int getOption();
	int freq(string t_item);
	void allFreq();
	void histogram();

};

#endif


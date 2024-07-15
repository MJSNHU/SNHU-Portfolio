#include "grocer.h"

#include <iostream>
using namespace std;


int main() {

	// backup input file
	backup();

	int input = -1;
	string item;

	Menu menu;


	// repeat until user quits
	while (input != 4) {
		input = menu.getOption();
		switch (input) {
		case 1:
			cout << "Enter item to search: ";
			cin >> item;
			cout << menu.freq(item) << endl;
			break;
		case 2:
			menu.allFreq();
			break;
		case 3:
			menu.histogram();
			break;
		default:
			continue;
		}
	}
	
	return 0;
}
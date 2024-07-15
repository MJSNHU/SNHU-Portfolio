
/*!

Matt Jackson
Project 3
CS-210
Hecker
SNHU

Corner grocer program:
Option 1:
	Prompt for item to search.
	Return number of times that item appears in input file.

Option 2:
	Print each item and the number of times they appear in the input file
	Input file is CS210_Project_Three_Input_File.txt.

Option 3:
	Print frequency of item by displaying an asterisk for each time the item appears in the input file

Option 3:
    Quit

Data File Creation:
  Creates frequency.dat which counts and saves the number of times each item appears in the input file
  This is used to process Option 2
*/

#include "grocer.h"

#include <iostream>
#include <fstream>
using namespace std;

const string INFILE = "CS210_Project_Three_Input_File.txt";
const string DATFILE = "frequency.dat";

/*!
 * Backup data from input file
 *
 * Inputs data from INFILE into map using freq(item) to count
 * frequency of each item
 * 
 * Writes map items to OUTFILE
 *  
 */
void backup() {

	map<string, int> groceryItems;
	ofstream outFile;
	fstream inFile;
	Menu menu;

	map <string, int>::iterator it = groceryItems.begin();

	cout << "Opening " << INFILE << " for reading ...";
	
	// try to open and process input file by saving into a map
	try {
    	inFile.open(INFILE);
		string line;
		while (getline(inFile, line)) {
			groceryItems.insert(make_pair(line, menu.freq(line)));
		}
		cout << "Done" << endl;

		inFile.close();
	}
	catch (exception& e) {
		cout << "\n" << e.what() << endl;
	}

	// try to open and write map data to output file; print exception if caught
	try {
		outFile.open(DATFILE);
		cout << "Backing up " << INFILE << " to " << DATFILE << "...";
		// iterate over map; on last map item do not print a newline 
		// this way the histogram function doesn't read a blank line
		int map_size = groceryItems.size();
		int i = 1;
		for (auto it : groceryItems) {
			if (i < map_size) {
				outFile << it.first << " " << it.second << endl;
			}
			else {
				outFile << it.first << " " << it.second;
			}
			i++;
		}
		outFile.close();
		cout << " Done" << endl;
	}
	catch (exception& e) {
		cout << "\n" << e.what() << endl;
    }

}	
/*!
*   Prints menu:
* 
*********************************
* 1) Frequency of item          *
* 2) Frequency of all items     *
* 3) Histogram                  *
* 4) Quit                       *
*********************************
*/
void Menu::printMenu() {
	
	cout.width(33);
	cout.fill('*');
	cout << "" << endl;
	cout.fill(' ');
	cout << "* 1) Frequency of item          *" << endl;
	cout << "* 2) Frequency of all items     *" << endl;
	cout << "* 3) Histogram                  *" << endl;
	cout << "* 4) Quit                       * " << endl;
	cout.width(33);
	cout.fill('*');
	cout << "" << endl;

}
/*! like stoi, but parses entire string, not just first integer characters
*  adapted from https://stackoverflow.com/questions/49635616/stdstoi-string-with-non-numeric-characters-getting-parsed-as-an-integer-with 
*  
*  stoi sets pos to the first position after the first integer characters
*  if pos != s.size(), there is a non integer character in the string, and 
*  function returns -1 
* 
*  Otherwise, function returns int value of s
* 
*  @param s String to parse
*/
int parseInt(const string& s) { 
	size_t pos; 
	int result = stoi(s, &pos); 
	
	// throw error if position marker is not at the end of the string
	if (pos != s.size()) {
		throw runtime_error("not all characters are integers");
		return -1;
	} 
	return result; 
}
/*!
*   Prompts user to input option until value is between 1 and 4
*   If selection is invalid, repeats prompt
*/
int Menu::getOption() {
	string input = "";
	int option = -1;

	printMenu();

	// input validation ensures only integers 1-4 will be accepted as input;
	// prompt repeats if not
	while  ((option > 4) || (option < 1)) {
		
		cout << "Please make your selection 1-4: ";
		
		cin >> input;

		// throw exceptions for invalid input
		try {
			option = parseInt(input);
			if ((option > 4) || (option < 1)) {
				throw (option);
			}
		} catch (int option){
			cout << option << ": invalid range" << endl;
		} catch (exception& e) {
			cout << e.what() << endl;
		}
	}
	return option;
}

/*!
*   Counts each instance of t_item in file, returns value as integer
* 
*   @param t_item Item to count
*/
int Menu::freq(string t_item) {

	int count = 0;

	fstream inFile;

	// try to open and process file, catch and print exceptions if fail
	try {
	    inFile.open(INFILE);
	    string line;
        while (getline(inFile, line)) {
		    if (t_item == line) {
			    count++;
		    }
	    }
	  inFile.close();

	}
	catch (exception& e) {
		cout << e.what() << endl;
	}

	return count;
}

/*!
*   Reads frequencies from DATFILE and outputs to screen
*/
void Menu::allFreq() {

	fstream inFile;
	
	// try to open and process .dat file
	// print exception if fail
	try {
	    inFile.open(DATFILE);
		string line;
		while (getline(inFile, line)) {
			cout << line << endl;
		}
		inFile.close();
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
	
}

/*!
*   Reads item and count from DATFILE
*   Outputs histogram with an * reprenting the count of each item
*/
void Menu::histogram() {

	fstream inFile;

	string item;
	string count;
	string line;

	try {
	    inFile.open(DATFILE);
		
		while (!inFile.eof()) {
			getline(inFile, item, ' ');
			getline(inFile, count, '\n');
			cout.fill(' ');
			cout << "  ";
			cout << left;
			cout.width(13);
			cout << item << " ";
			cout.width(stoi(count));
			cout.fill('*');
			cout << "" << endl;
			cin;
		}
		inFile.close();
	} catch (exception& e) {
		cout << e.what() << endl;
	}
}

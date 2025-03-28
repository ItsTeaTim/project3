// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	buildMap();
}

// Constructor – pass bible filename
Bible::Bible(const string s) { 
	infile = s; 
	buildMap();
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
	Verse verse;
	
	string verseText = "";

	if (!instream.is_open()) {
		instream.open(infile, ios::binary);
		if (!instream) {
			return verse;
		}
	}

	auto it = bibRef.find(ref);
	instream.clear();  
	instream.seekg(it->second, ios::beg); 

	getline(instream, verseText);
	if (verseText.empty()) {
		status = NO_VERSE;
		return verse;
	}

	Verse errHandle(verseText);
	Ref handle = errHandle.getRef();

	// Checks for validity
	if (ref.getBook() > 66 || ref.getBook() <= 0) {
		status = NO_BOOK;
		return verse;
	}
	else if (handle.getBook() == ref.getBook() && ref.getChap() != handle.getChap()) {
		status = NO_CHAPTER;
		return verse;
	}
	else if (handle.getBook() == ref.getBook() && ref.getChap() == handle.getChap() && handle.getVerse() != ref.getVerse()) {
		status = NO_VERSE;
		return verse;
	}

	status = SUCCESS;
	return Verse(verseText);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
	Verse nextVerse;

	// If file is not open, open it
	if (!instream.is_open()) {
		instream.open(infile, ifstream::in);
		if (!instream) {  
			status = NO_BOOK;
			cout << error(status);
			return nextVerse;
		}
	}

	string nextLine;
	if (getline(instream, nextLine)) {
		status = SUCCESS;
		return Verse(nextLine);  
	}

	status = OTHER;
	return nextVerse;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	string err;
	if (status == NO_BOOK) {
		err = "Error: no valid book found";
		return err;
	}
	else if (status == NO_CHAPTER) {
		err = "Error: no valid chapter found";
		return err;
	}
	else if (status == NO_VERSE) {
		err = "Error: no valid verse found";
		return err;
	}
	else if (status == OTHER) {
		err = "Error: other issue occurred";
		return err;
	}
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status) {
	auto it = bibRef.find(ref);
	if (it == bibRef.end()) {
		status = NO_VERSE;
		return Ref();
	}

	it++;
	if (it == bibRef.end()) {
		status = OTHER;  // No next verse found
		return Ref();
	}

	status = SUCCESS;
	return it->first; // Return the next reference
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status) {}

void Bible::buildMap() {

	ifstream file(infile);
	string str = "";
	int byteOffset = 0;

	while (getline(file, str)) {
		Ref ref(str);
		bibRef.insert({ ref, byteOffset });
		byteOffset = file.tellg();
	}

	file.close();
}



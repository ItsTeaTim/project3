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
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
    // TODO: scan the file to retrieve the line that holds ref ...
    // update the status variable
	status = OTHER; // placeholder until retrieval is attempted
	// create and return the verse object
	Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.
	
	if (!instream.is_open()) {
		instream.open(infile, ifstream::in);
	}
	string line;
	Verse verse;
	if (ref.getBook() > 66) {
		status = NO_BOOK;
		return(aVerse);
	}
	while (!instream.eof()) {
		getline(instream, line);
		verse = Verse(line);
		//verse.display();
		if (verse.getRef() == ref) { // If the verses are equal, return the verse
			status = SUCCESS;
			return verse;
		}
		else if (ref.getBook() < verse.getRef().getBook()) {
			status = NO_CHAPTER;
			return(aVerse);
		}
		else if (ref.getChap() < verse.getRef().getChap() && ref.getBook() == verse.getRef().getBook()) {
			status = NO_VERSE;
			return(aVerse);
		}
		else {
			status = OTHER;
		}
	} 
	return(aVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
	Verse next;
	string nextLine;
	if (!instream.is_open()) {
		instream.open(infile, ifstream::in);
		next = Verse("1 1 1");
		status = SUCCESS;
		return next;
	}
	while (!instream.eof()) {
		getline(instream, nextLine);
		next = Verse(nextLine);
		status = SUCCESS;
		return next;
	}
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
Ref Bible::next(const Ref ref, LookupResult& status) {}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status) {}

void Bible::buildMap() {

	ifstream file(infile);
	string str = "";
	int byteOffset = 0;

	while (getline(file, str)) {
		Ref ref(str);
		byteOffset = file.tellg();
		bibRef.insert(<ref, byteOffset>);
	}

	file.close();
}

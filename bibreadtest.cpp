#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	webBible.buildMap();

	Ref ref1(1, 1, 1);
	Ref ref2(45, 3, 9);
	Ref ref3(23, 34, 12);

	cout << bibRef[ref1] << endl;
	cout << bibRef[ref2] << endl;
	cout << bibRef[ref3] << endl;
}
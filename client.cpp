/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 * 
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching 
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Bible.h"
#include "Ref.h"
#include "Verse.h"
#include "fifo.h"
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

int main() {
	cout << "Content-Type: text/plain\n\n";

	Fifo requestFifo("request_pipe");
	requestFifo.openwrite();

	Fifo replyFifo("reply_pipe");
	replyFifo.openread();


	Bible webBible("/home/class/csc3004/Bibles/web-complete");
  /* A CGI program must send a response header with content type
   * back to the web client before any other output.
   * For an AJAX request, our response is not a complete HTML document,
   * so the response type is just plain text to insert into the web page.
   */
  
  Cgicc cgi;  // create object used to access CGI request data

  // GET THE INPUT DATA
  // browser sends us a string of field name/value pairs from HTML form
  // retrieve the value for each appropriate field name
  form_iterator st = cgi.getElement("search_type");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");

  // Convert and check input data

  /*bool validInput = false;
  if (chapter != cgi.getElements().end()) {
	 int chapterNum = chapter->getIntegerValue();
	 if (chapterNum > 150) {
		 cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
	 }
	 else if (chapterNum <= 0) {
		 cout << "<p>The chapter must be a positive number.</p>" << endl;
	 }
	 else
		 validInput = true;
  }*/
  
  /* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */

  /*bool validVerse = false;
  if (verse != cgi.getElements().end()) {
	  int verseNum = verse->getIntegerValue();
	  if (verseNum > 176) {
		  cout << "<p>The verse number (" << verseNum << ") is too high.</p>" << endl;
	  }
	  else if (verseNum <= 0) {
		  cout << "<p>The verse must be a positive number.</p>" << endl;
	  }
	  else
		  validVerse = true;
  }*/


  /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   *        TO LOOK UP THE REQUESTED VERSES
   */
  int bookResult = book->getIntegerValue();			
  int chapResult = chapter->getIntegerValue();
  int verseResult = verse->getIntegerValue();
  int numVerses = nv->getIntegerValue();


  // Make the ref for the server
  string request = to_string(bookResult) + " " + to_string(chapResult) + " " + to_string(verseResult) + " " + to_string(numVerses);

  // Set up pipes to do things
  
  requestFifo.send(request);
  string response = replyFifo.recv();
  cout << "<p>" + response + "</p>" << endl;

  requestFifo.fifoclose();
  replyFifo.fifoclose();

  /*if (numVerses <= 0) {			// If the number of verses is empty, set it to 1
	  numVerses = 0;
  }

  bool validBook = false;
  string bookName = "";
  if (bookResult <= 66 && bookResult >= 1)
  {
	 validBook = true;
	 bookName = books[bookResult];
  }
  else {
	  validBook == false;
  }

  Ref ref = Ref(bookResult, chapResult, verseResult);
  LookupResult result;
  Verse lookVerse = webBible.lookup(ref, result);
  string err = webBible.error(result);

  if (err == "Error: no valid book found") {
	  validBook = false;
	  cout << "<b>"  << err << "</b>"  << endl;
  }
  else if (err == "Error: no valid chapter found") {
	  validInput = false;
	  cout << "<b>" << err << "</b>" << endl;
  }
  else if (err == "Error: no valid verse found") {
	  validVerse = false;
	  cout << "<b>" << err << "</b>" << endl;
  }
  else if (err == "Error: other issue occurred") {
	  cout << "<b>" << err << "</b>" << endl;
  }

  /* SEND BACK THE RESULTS
   * Finally we send the result back to the client on the standard output stream
   * in HTML text format.
   * This string will be inserted as is inside a container on the web page, 
   * so we must include HTML formatting commands to make things look presentable!
   */
  /*if (validBook && validInput && validVerse) {
	cout << "Search Type: <b>" << **st << "</b>" << endl;
	cout << "<p>Your result: "
		 << bookName << " " << chapResult << ":" << verseResult 
		 << "<p> <em>" << lookVerse.getVerse() << "</em> </p>" << endl;

	for (int i = 0; i < numVerses - 1; i++) {				// For additional verses
		lookVerse = webBible.nextVerse(result);
		bookResult = lookVerse.getRef().getBook();    // Update data
		bookName = books[bookResult];
		chapResult = lookVerse.getRef().getChap();
		verseResult = lookVerse.getRef().getVerse();

		cout << "<p>" << bookName << " " << chapResult << ":" << verseResult
			<< "<p> <em>" << lookVerse.getVerse() << "</em> </p>" << endl;
	}
  }*/
  return 0;
}

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

int main(int argc, char** argv) {

    Fifo requestFifo("request_pipe");
    requestFifo.openread();

    Fifo replyFifo("reply_pipe");
    replyFifo.openwrite();

    Bible webBible("/home/class/csc3004/Bibles/web-complete");

    cout << "Lookup Server started, waiting for requests..." << endl;

    while (true) {
        // Receive request
        string request = requestFifo.recv();
        cout << "Received request: " << request << endl;

        int book, chapter, verse, nums;
        istringstream requestStream(request);

        if (request.empty()) {
            cout << "No request received." << endl;
        }

        // Parse that junk
        if (!(requestStream >> book >> chapter >> verse >> nums)) {
            cout << "Invalid request format." << endl;
            continue;
        }

        if (nums < 0) {
            cout << "The number of verses to retrieve should be positive." << endl;
            continue;
        }

        Ref ref(book, chapter, verse);
        LookupResult status;
        Verse verseObj = webBible.lookup(ref, status);
        string reply;
        string bookName = books[book];

        // Lookup first verse
        if (status == LookupResult::SUCCESS) {
            string verseText = verseObj.getVerse();
            reply = bookName + " " + to_string(ref.getChap()) + ":" + to_string(ref.getVerse()) + ": " + verseText;
        }
        else {
            reply = "Error retrieving request: " + webBible.error(status);
            replyFifo.send(reply);
            continue;
        }

        if (status == LookupResult::SUCCESS) {
            // Retrieve additional verses if requested
            for (int i = 1; i < nums; i++) {
                verseObj = webBible.nextVerse(status);
                ref = verseObj.getRef();
                if (status == LookupResult::SUCCESS) {
                    string verseText = verseObj.getVerse();
                    reply += " " + bookName + " " + to_string(ref.getChap()) + ":" + to_string(ref.getVerse()) + " " + verseText;
                }
                else {
                    cout << "Error retrieving request: " << webBible.error(status) << endl;
                }
            }
            cout << "Sending reply:\n" << reply << endl; // Prints reply for debugging
            replyFifo.send(reply);
        }
    }
    requestFifo.fifoclose();
    replyFifo.fifoclose();
    return 0;
}
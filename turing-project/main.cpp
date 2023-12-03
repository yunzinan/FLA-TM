#include <iostream>
#include <string>
#include "tm.hpp"

using namespace std;



string helpMsg = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";

bool arguementHelper(int cnt, int argc) {
    if (argc == cnt) return true; 
    else if (argc > cnt) {
        cerr << "too much arguments. see usage.\n" << helpMsg << endl;
        return false;
    }
    else {
        cerr << "too few arguments. see usage.\n" << helpMsg << endl;
        return false;
    }
}


int main(int argc, char* argv[]){
    // do some interactive jobs
    /**
     * valid input example:
     * turing --help
     * turing -h <tm> <input>
     * turing --help sadfjo asdoifja s sadfj oasdf sadf ...
     * turing --verbose <tm> <input>
     * turing <tm> <input>
     */ 
    if(argc == 1){
        // so no argument input, the only arg is the relative path of the file
        cerr << "too few arguments. see usage.\n" << helpMsg << endl;
        return 1;
    }
    // instantiate a turing machine
    TuringMachine tm = TuringMachine();
    Log("%d arguments received:", argc);
    for (int i = 0; i < argc; i++) {
        Log("%s", argv[i]);
    }
    if (string(argv[1]) == "-v" || string(argv[1]) == "--verbose") {
        if (arguementHelper(4, argc)) {
            tm.setVerbose();
            if (tm.loadConfig(argv[2])) {
                tm.run(argv[3]);
            }
        }
    } 
    else if (string(argv[1]) == "-h" || string(argv[1]) == "--help") {
        cout << helpMsg << endl;
    }
    else {
        if (arguementHelper(3, argc)) {
            if (tm.loadConfig(argv[1])) {
                tm.run(argv[2]);
            }
        }
    }
    return 0;
}
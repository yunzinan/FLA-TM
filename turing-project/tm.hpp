#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
// this is used for keeping track of the running status of the prog. all unnecessary output will use Log(...)
// note that in verbose mode, all output will use printf/cout/cerr
#define DEBUG

#ifdef DEBUG
    #define Log(format, ...) \
	        do { \
		        fprintf(stdout, "\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
				        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
		        fflush(stdout); \
	        } while(0)
#else
    #define Log(format, ...) 
#endif

typedef struct trans{
    std::string sym; // how the symbols on tapes change
    std::string mov; // how the tapes move
    std::string nxtState; // what is the next state
}trans;

class TuringMachine{
    bool verbose = false;
    // the config
    std::vector<std::string> _Q; // the set of states
    std::vector<std::string> _S; // the set of input symbols
    std::vector<std::string> _G; // the set of tape symbols
    std::string _q0; // the starting state
    std::string _B; // the blank symbol
    std::vector<std::string> _F; // the set of final states 
    int _N = -1; // the number of tapes
    std::map<std::string, std::map<std::string, trans>> _trans; // the transition function
    // usage: _trans['stateName1']['input syms'] = {'output symgs', 'movs'}

public:
    TuringMachine() {
        Log("turing machine started!");
    }
    void setVerbose() {
        Log("set verbose mode.");
        this->verbose = true;
    }
    bool loadConfig(std::string tmPath); // return true if successfully loaded <tm>; false otherwise.
    void run(std::string input); // the wrapper function of the simulation
    void step(); // one single step the turing machine
    void printConfig(); // for debug use: print the def of the TM
};

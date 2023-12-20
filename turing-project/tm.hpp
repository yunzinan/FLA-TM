/**
 * this is the project of NJU Formal Language and Automata 2023 Fall course 
 * @author: yunzinan
 * @date: 2023/12/5
 * @brief: the header file of the class TuringMachine
*/


#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <string>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
// this is used for keeping track of the running status of the prog. all unnecessary output will use Log(...)
// note that in verbose mode, all output will use printf/cout/cerr
// #define DEBUG


#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)


#ifdef DEBUG
    #define Log(format, ...) \
	        do { \
		        fprintf(stdout, "\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
				        __FILENAME__, __LINE__, __func__, ## __VA_ARGS__); \
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
    std::set<std::string> _Q; // the set of states
    std::set<std::string> _S; // the set of input symbols
    std::set<std::string> _G; // the set of tape symbols
    std::string _q0; // the starting state
    std::string _B; // the blank symbol
    std::set<std::string> _F; // the set of final states 
    int _N = -1; // the number of tapes
    std::map<std::string, std::map<std::string, trans>> _trans; // the transition function
    // usage: _trans['stateName1']['input syms'] = {'output symgs', 'movs'}
    // runtime infos
    std::string __input;
    std::string __state; 
    std::vector<std::deque<std::string>> __tape; //tape[i][j] means the ith tape the jth index: should be a char 
    std::vector<int> __left; // to keep track of the leftmost place of each tape
    std::vector<int> __head; // which points out the cur pos of each tape
    int __step; // the counter of running steps
    std::string __acc; // whether the turing machine accepts now, "acc", "rej", 'run'
    bool __halt = false;
    trans findTrans(std::string state, std::string sym); //find the exact trans of current status, mainly deal with asterisk
    

public:
    TuringMachine() {
        Log("turing machine started!");
    }
    void setVerbose() {
        Log("set verbose mode.");
        this->verbose = true;
    }
    bool loadConfig(std::string tmPath); // return true if successfully loaded <tm>; false otherwise.
    void checkInput(std::string input); // check whether the input is valid
    void run(std::string input); // the wrapper function of the simulation
    void step(); // one single step the turing machine
    void printConfig(); // for debug use: print the def of the TM
    void printStep(); // print out the info of current TM
    void printSingleTape(int index); // for the need of alignment, will be done further, as not essential
    void initTape(std::string input); // init the tapes given input 
    void printSyntaxErr(); // for the robustness of loadConfig: if missing '{', '}', ',', '#'
    bool preCheck(std::string input); // just do the same thing as checkInput

};

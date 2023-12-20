/**
 * this is the project of NJU Formal Language and Automata 2023 Fall course 
 * @author: yunzinan
 * @date: 2023/12/5
 * @brief: the implementation of class TuringMachine
*/



#include "tm.hpp"

using namespace std;

// omit the comment by the end of a line
string transform(string str) {
    int len = str.length();
    int index = -1;
    for (int i = 0; i < len; i++) {
        if (str[i] == ';') {
            index = i;
            break;
        }
    }
    if (index != -1) {
        str = str.substr(0, index);
    }
    return str;
}

vector<string> split(string &str, char splitter) {
    vector<string> res;
    stringstream ss(str);
    string item;
    while(getline(ss, item, splitter)) {
        res.push_back(item);
    }
    return res;
}

bool TuringMachine::loadConfig(string tmPath) {
    Log("loading file, file path: %s", tmPath.c_str());         
    
    ifstream file(tmPath);

    if (file) {
        string line;
        vector<string> tmp; 
        // Q, S, G, q0, B, F, N, trans
        while (getline(file, line)) {
            if (line.length() == 0) continue; // omit the blank lines
            if (line[0] == ';') continue; // omit the comments
            line = transform(line);
            Log("%s", line.c_str());

            // #Q = {0,accept,B,C,D}
            if (this->_Q.empty()) {
                tmp = split(line, ' '); 
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                // tmp[2] = '{0,acc,1,this,rej}'
                if (tmp[0] != "#Q") printSyntaxErr(); // check1: should be #Q
                if (tmp[2][0] != '{' || tmp[2][tmp[2].length() - 1] != '}') printSyntaxErr(); // check2: should be branked by {}
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                set<string> temp;
                tmp = split(s, ',');
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                    temp.insert(tmp[i]);
                } 
                this->_Q = temp;
                continue;
            }
            // #S = {0,1}
            else if (this->_S.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                if (tmp[0] != "#S") printSyntaxErr(); // check1: should be #S
                if (tmp[2][0] != '{' || tmp[2][tmp[2].length() - 1] != '}') printSyntaxErr(); // check2: should be branked by {}
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                set<string> temp;
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                    if (tmp[i].length() != 1) printSyntaxErr(); // check3: each symbol should be exactly 1 character
                    temp.insert(tmp[i]);
                } 
                this->_S = temp;
                continue;
            }
            // #G = {0,1,_,t,r,u,e,f,a,l,s}
            else if (this->_G.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                if (tmp[0] != "#G") printSyntaxErr(); // check1: should be #G
                if (tmp[2][0] != '{' || tmp[2][tmp[2].length() - 1] != '}') printSyntaxErr(); // check2: should be branked by {}
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                set<string> temp;
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                    if (tmp[i].length() != 1) printSyntaxErr(); // check3: each symbol should be exactly 1 character
                    temp.insert(tmp[i]);
                } 
                this->_G = temp;
                continue;
            }
            // #q0 = 0
            else if (this->_q0.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                if (tmp[0] != "#q0") printSyntaxErr(); // check1: should be #q0
                this->_q0 = tmp[2];
                continue;
            }
            // #B = _
            else if (this->_B.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                if (tmp[0] != "#B") printSyntaxErr(); // check1: should be #B
                this->_B = tmp[2];
                continue;
            }
            // #F = {halt_accept,wtf}
            else if (this->_F.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                if (tmp[0] != "#F") printSyntaxErr(); // check1: should be #F
                if (tmp[2][0] != '{' || tmp[2][tmp[2].length() - 1] != '}') printSyntaxErr(); // check2: should be branked by {}
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                set<string> temp;
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                    temp.insert(tmp[i]);
                } 
                this->_F = temp;
                continue;
            } 
            // #N = 2
            else if (this->_N == -1) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                if (tmp[0] != "#N") printSyntaxErr(); // check1: should be #N
                this->_N = stoi(tmp[2]);
                continue;
            }
            // trans
            // cp 0_ 00 rr cp
            else {
                tmp = split(line, ' ');
                Log("%s, %s, %s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str(),
                    tmp[3].c_str(), tmp[4].c_str());
                string state = tmp[0];
                string cursym = tmp[1];
                trans cur;
                cur.sym = tmp[2];
                cur.mov = tmp[3];
                cur.nxtState = tmp[4];
                this->_trans[state][cursym] = cur;
                if (this->_Q.find(state) == this->_Q.end() || this->_Q.find(cur.nxtState) == this->_Q.end()) {
                    if (this->_Q.find(state) == this->_Q.end()) {
                        Log("%s missing in definition!", state.c_str());
                    }
                    if (this->_Q.find(cur.nxtState) == this->_Q.end()) {
                        Log("%s missing in definition!", cur.nxtState.c_str());
                    }
                    cerr << "new state found in transition function!\n";
                    exit(-1);
                }
            }
        }

        file.close();
    }
    else {
        cerr << "illegal tm file path" << endl;
        // return false;
        exit(-1);
    }
    this->printConfig();
    return true;
}


void TuringMachine::printConfig() {
    Log("======== Config =========");  
    // Q
    Log("----------  Q  ----------");
    for (auto it = this->_Q.begin(); it != this->_Q.end(); it++) {
        Log("%s", (*it).c_str());
    }
    // S
    Log("----------  S  ----------");
    for (auto it = this->_S.begin(); it != this->_S.end(); it++) {
        Log("%s", (*it).c_str());
    }
    // G
    Log("----------  G  ----------");
    for (auto it = this->_G.begin(); it != this->_G.end(); it++) {
        Log("%s", (*it).c_str());
    }
    // q0
    Log("----------- q0 ----------");
    Log("%s", this->_q0.c_str());
    // B
    Log("----------  B  ----------");
    Log("%s", this->_B.c_str());
    // F
    Log("----------  F  ----------");
    for (auto it = this->_F.begin(); it != this->_F.end(); it++) {
        Log("%s", (*it).c_str());
    }
    // N
    Log("----------  N  ----------");
    Log("%d", this->_N);
    // trans
    Log("----------  Q  ----------");
    for (auto it = this->_trans.begin(); it != this->_trans.end(); ++it) {
        string curState = it->first;
        map<string, trans> value = it->second;
        for (auto it2 = value.begin(); it2 != value.end(); ++it2) {
            string curSym = it2->first;
            trans cur = it2->second;
            Log("%s %s %s %s %s", curState.c_str(), curSym.c_str(), 
                cur.sym.c_str(), cur.mov.c_str(), cur.nxtState.c_str());            
        }
        
    } 
    Log("========== END ==========");  
}


void TuringMachine::step() {
    // step 1: find current state and get the transition 
    // 1.1 combine the headers to make the inSym
    string inSym; // the input symbols of each tape where their headers point to
    for (int i = 0; i < this->_N; i++) {
        int pos = this->__head[i];
        inSym += this->__tape[i][pos];
    }
    // then get the outSym, mov, nxtState
    trans nxt = this->findTrans(this->__state, inSym); 
    // trans nxt = this->_trans[this->__state][inSym];
    if (this->__acc == "rej") {
        return ;
    }
    string outSym = nxt.sym;
    string _nxtState = nxt.nxtState;
    string mov = nxt.mov;
    Log("curState: %s, inSym: %s | outSym: %s, mov: %s, nxtState: %s",
                                     this->__state.c_str(), inSym.c_str(), 
                                     outSym.c_str(), mov.c_str(), _nxtState.c_str());
    if (this->_Q.find(this->__state) == this->_Q.end()) {
        Log("Error: cur state [%s] not found in the list! check the program case!", this->__state.c_str());
    }
    // step 2: make the transition
    // 2.1 change the tape 
    for (int i = 0; i < this->_N; i++) {
        this->__tape[i][this->__head[i]] = outSym[i];
    }
    // 2.2 make the move
    for (int i = 0; i < this->_N; i++) {
        string blank = "_";
        switch (mov[i])
        {
        case 'l':
            this->__head[i]--;
            if (this->__head[i] < 0) {
                this->__head[i] = 0;
                this->__tape[i].push_front(blank);
                this->__left[i]++;
            }
            break;
        case 'r': 
            this->__head[i]++;
            if (this->__head[i]>= this->__tape[i].size()) {
                this->__tape[i].push_back(blank);
            }
            break;
        case '*':
            // so don't move
            break;
        default:
            // should not go there
            Log("[error]: invalid move!");
            exit(-1);
            break;
        }
    }
    // 2.3 change the state
    this->__state = _nxtState;
    if (this->_F.find(this->__state) != this->_F.end()) {
        this->__acc = "acc";
        return ;
    }
}

trans TuringMachine::findTrans(string state, string sym) {
    // first case, if there exists the exact match, then bingo!
    trans ret;
    auto it = this->_trans[state].find(sym);
    if (it != this->_trans[state].end()) {
        return it->second;
    }
    //second case, try to find if there is a wider rule for that 
    else {
        bool flag = false;
        for (auto it = this->_trans[state].begin(); it != this->_trans[state].end(); it++) {
            string ruleSym = it->first;
            ret = it->second;
            // e.g.  "ADC_" -> "A*C_"
            // goal: if the cursym, replace it with the current one
            string tmp = ruleSym;
            // replace the rule of * by sym
            for (int i = 0; i < tmp.length(); i++) {
                if (tmp[i] == '*' && sym[i] != '_') {
                    tmp[i] = sym[i];
                    if (ret.sym[i] == '*') {
                        ret.sym[i] = sym[i];
                    }
                }
            }  
            if (tmp == sym) {
                flag = true;
                return ret;
            }
        }
        if (!flag) {
            // third case, no rule for the current status, boom
            Log("[error]: no matching rules found!");
            this->__acc = "rej";
        }
    }
    // should not go there
    return ret;
}


void TuringMachine::initTape(string input) {
    // we can always assmue that in the step 0, only 0th tape has non-blank info
    this->__tape.resize(this->_N);
    this->__head.resize(this->_N);
    this->__left.resize(this->_N);
    for (int i = 0; i < this->_N; i++) {
        this->__head[i] = 0;
        this->__left[i] = 0;
    }
    // Log("[breakpoint]");
    // init the 0th tape
    // input e.g.: 1001001
    for (int i = 0; i < input.length(); i++) {
        string curSym = input.substr(i, 1);
        this->__tape[0].push_back(curSym);
    }
    //XXX: special case: if the input string is empty, then need to push a blank
    if (input.length() == 0) {
        this->__tape[0].push_back(this->_B);
    }
    // now deal with the others
    for (int i = 1; i < this->_N; i++) {
        this->__tape[i].push_back(string("_"));
    }
}

void TuringMachine::printStep() {
    /**
     *  ---------------------------------------------
        Step   : 5
        State  : cp
        Acc    : No
        Index0 : 0 1 2 3 4 5 6
        Tape0  : 1 0 0 1 0 0 1
        Head0  :         ^
        Index1 : 0 1 2 3 4
        Tape1  : 1 0 0 1 _
        Head1  :         ^
        ---------------------------------------------
     */

    string _step      = "Step   : " + to_string(this->__step) + "\n";
    string _state     = "State  : " + this->__state + "\n"; 
    string _acc;
    if (this->__acc == "acc") {_acc = "Acc    : Yes\n";}
    else {_acc =             "Acc    : No\n";}
    printf("%s", _step.c_str());
    printf("%s", _state.c_str());
    printf("%s", _acc.c_str()); 
    for(int i = 0; i < this->_N; i++) {
        printSingleTape(i);
    }


    printf("---------------------------------------------\n"); 
}

void TuringMachine::printSingleTape(int index) {
    deque<string> &curTape = this->__tape[index];
    int n = curTape.size();
    string _index = "Index" + to_string(index) + " :";
    printf("%s", _index.c_str());
    // job1: change the indices(specially, the -3 -2 -1 case)
    /*
    e.g. this->__left[0] = 3,
         curTape.size(i.e., n) = 6
         then, the correct indices should be:
         3 2 1 0 1 2
    */
    vector<int> indices;
    indices.resize(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i - this->__left[index]; // 0 - 3 = -3
        if (indices[i] < 0) indices[i] = -indices[i];
    } 
    // job2: omit the blanks
    int l = 0, r = 0; // l should be min (first non-blank sym, header), r should be max(last non-blank sym, header[index])
    // find the l
    for (int i = 0; i < n; i++) {  
        if (curTape[i] != this->_B) {
            l = i;
            break;
        }
        else {
            l++;
        }
    }
    if (this->__head[index] < l) {l = this->__head[index];};
    // find the r 
    for (int i = n-1; i >= 0; i--) {
        if (curTape[i] != this->_B) {
            r = i;
            break;
        }
        else {
            r--;
        }
    }
    if (this->__head[index] > r) r = this->__head[index];
    for (int i = l; i <= r; i++) {
        printf(" %d", indices[i]);
    }
    printf("\n");
    // Log("head: %d, l: %d, r: %d", this->__head[index], l, r);
    string _tape = "Tape" + to_string(index) + "  :";
    printf("%s", _tape.c_str());
    for (int i = l; i <= r; i++) {
        if(to_string(indices[i]).length() == 1 || indices[i] == 10) printf(" %s", curTape[i].c_str()); // XXX: special case: '10'
        else {
            // XXX: note: here we assume that indices cannot go beyond -99~99, i.e. use no more than 2 bit to represent
            // XXX: special case: the first one should always be one blank before
            if (i == l) {
                printf(" %s", curTape[i].c_str());
            }
            else printf("  %s", curTape[i].c_str());
        }
    }
    printf("\n");
    // job3: set the correct header pos
    string tmp;
    for (int i = l; i < this->__head[index]; i++) {
        if(to_string(indices[i]).length() == 1) tmp += "  "; // XXX: tricky things: here we don't need to consider '10', cause we need to go over it
        else {
            // XXX: note: here we assume that indices cannot go beyond -99~99, i.e. use no more than 2 bit to represent
            tmp += "   ";
        }
    }
    string _head = "Head" + to_string(index) + "  : " + tmp + "^";
    printf("%s", _head.c_str());
    printf("\n");
}

void TuringMachine::run(string input) {
    // Log("[breakpoint]");
    this->__input = input;
    this->initTape(this->__input);
    this->__state = this->_q0;
    this->__step = 0;
    this->__acc = "run";
    if (this->verbose) {
        printf("Input: %s\n", this->__input.c_str());
        checkInput(input);
        printf("==================== RUN ====================\n");
    }
    else checkInput(input);
    do {
        if(this->verbose) printStep();
        step();
        this->__step++;
    }while(this->__acc == "run");
    if (this->verbose) {
        if (this->__acc == "acc") {
            printStep();
            printf("ACCEPTED\n");
        }
        else {
            // "rej"
            printStep();
            printf("UNACCEPTED\n");
        }
        printf("Result: ");
        int l = 0, r = 0; // l should be min (first non-blank sym, header), r should be max(last non-blank sym, header[index])
        // find the l
        int n = this->__tape[0].size();
        for (int i = 0; i < n; i++) {  
            if (this->__tape[0][i] != this->_B) {
                l = i;
                break;
            }
            else {
                l++;
            }
        }
        if (this->__head[0] < l) {l = this->__head[0];};
        // find the r 
        for (int i = n-1; i >= 0; i--) {
            if (this->__tape[0][i] != this->_B) {
                r = i;
                break;
            }
            else {
                r--;
            }
        }
        if (this->__head[0] > r) r = this->__head[0];
        for (int i = l; i <= r; i++) {
            cout << this->__tape[0][i];
        }
        printf("\n");
        printf("==================== END ====================\n");
    }
    else {
        int l = 0, r = 0; // l should be min (first non-blank sym, header), r should be max(last non-blank sym, header[index])
        // find the l
        int n = this->__tape[0].size();
        for (int i = 0; i < n; i++) {  
            if (this->__tape[0][i] != this->_B) {
                l = i;
                break;
            }
            else {
                l++;
            }
        }
        if (this->__head[0] < l) {l = this->__head[0];};
        // find the r 
        for (int i = n-1; i >= 0; i--) {
            if (this->__tape[0][i] != this->_B) {
                r = i;
                break;
            }
            else {
                r--;
            }
        }
        if (this->__head[0] > r) r = this->__head[0];
        if (this->__acc == "acc") {
            cout << "(ACCEPTED) ";
            for (int i = l; i <= r; i++) {
                cout << this->__tape[0][i];
            }
            cout << "\n";
        }
        else {
            // "rej"
            cout << "(UNACCEPTED) ";
            for (int i = l; i <= r; i++) {
                cout << this->__tape[0][i];
            }
            cout << "\n";
        }
    }
}

void TuringMachine::checkInput(string input) {
    // check if there exists any invalid input, that is not in the _S
    int pos = -1;
    for(int i = 0; i < input.length(); i++) {
        auto it = this->_S.find(string(1, input[i]));
        if (it == this->_S.end()) {
            // so not find
            pos = i; 
        }
    }
    if (pos != -1) {
        if (this->verbose) {
            cerr << "==================== ERR ====================\n"; 
            string s(1, input[pos]);
            string errMsg = "error: Symbol \"" + s 
            + "\" in input is not defined in the set of input symbols\n";
            cerr << errMsg;
            string inputMsg = "Input: " + input + "\n";
            cerr << inputMsg;
            for (int i = 0; i < 7 + pos; i++) {
                // printf(" ");
                cerr << " ";
            }
            // printf("^\n");
            cerr << "^\n";
            cerr << "==================== END ====================\n";
        }
        else {
            cerr << "illegal input string\n";
        }
        exit(-1);
    }
}

void TuringMachine::printSyntaxErr() {
    cerr << "syntax error\n";
    exit(-1);
}
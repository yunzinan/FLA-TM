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
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                } 
                this->_Q = tmp;
                continue;
            }
            // #S = {0,1}
            else if (this->_S.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                } 
                this->_S = tmp;
                continue;
            }
            // #G = {0,1,_,t,r,u,e,f,a,l,s}
            else if (this->_G.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                } 
                this->_G = tmp;
                continue;
            }
            // #q0 = 0
            else if (this->_q0.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                this->_q0 = tmp[2];
                continue;
            }
            // #B = _
            else if (this->_B.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                this->_B = tmp[2];
                continue;
            }
            // #F = {halt_accept,wtf}
            else if (this->_F.empty()) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
                string s = tmp[2].substr(1, tmp[2].length() - 2);
                tmp = split(s, ',');
                for (int i = 0; i < tmp.size(); i++) {
                    Log("%s", tmp[i].c_str());
                } 
                this->_F = tmp;
                continue;
            } 
            // #N = 2
            else if (this->_N == -1) {
                tmp = split(line, ' ');
                Log("%s, %s, %s", tmp[0].c_str(), tmp[1].c_str(), tmp[2].c_str());
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
            }
        }

        file.close();
    }
    else {
        cerr << "illegal tm file path." << endl;
        return false;
    }
    this->printConfig();
    return true;
}

void TuringMachine::printConfig() {
    Log("======== Config =========");  
    // Q
    Log("----------  Q  ----------");
    for (int i = 0; i < this->_Q.size(); i++) {
        Log("%s", this->_Q[i].c_str());
    }
    // S
    Log("----------  S  ----------");
    for (int i = 0; i < this->_S.size(); i++) {
        Log("%s", this->_S[i].c_str());
    }
    // G
    Log("----------  G  ----------");
    for (int i = 0; i < this->_G.size(); i++) {
        Log("%s", this->_G[i].c_str());
    }
    // q0
    Log("----------- q0 ----------");
    Log("%s", this->_q0.c_str());
    // B
    Log("----------  B  ----------");
    Log("%s", this->_B.c_str());
    // F
    Log("----------  F  ----------");
    for (int i = 0; i < this->_F.size(); i++) {
        Log("%s", this->_F[i].c_str());
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

}


void TuringMachine::initTape(string input) {
    // we can always assmue that in the step 0, only 0th tape has non-blank info
    this->__tape.resize(this->_N);
    this->__head.resize(this->_N);
    for (int i = 0; i < this->_N; i++) {
        this->__head[i] = 0;
    }
    // init the 0th tape
    // input e.g.: 1001001
    for (int i = 0; i < input.length(); i++) {
        string curSym = input.substr(i, 1);
        this->__tape[0].push_back(curSym);
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
    if (this->__acc) {_acc = "Acc    : Yes\n";}
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
    string _index = "Index" + to_string(index) + " : ";
    printf("%s", _index.c_str());
    // TODO: to be fix in the future
    for (int i = 0; i < n; i++) {
        printf("%d ", i);
    }
    printf("\n");
    string _tape = "Tape" + to_string(index) + "  : ";
    printf("%s", _tape.c_str());
    for (int i = 0; i < n; i++) {
        printf("%s ", curTape[i].c_str());
    }
    printf("\n");
    string _head = "Head" + to_string(index) + "  : " + to_string(this->__head[index]);
    printf("%s", _head.c_str());
    printf("\n");
}

void TuringMachine::run(string input) {
    this->__input = input;
    this->initTape(this->__input);
    this->__state = this->_q0;
    this->__step = 0;
    this->__acc = false;
    printf("Input: %s\n", this->__input.c_str());
    printf("==================== RUN ====================\n");
    printStep();
    // do {
    //     step();
    // }while(!this->__acc);
}
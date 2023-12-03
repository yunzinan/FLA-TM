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

void TuringMachine::run(string input) {
    Log("start running on input %s", input.c_str());

}

void TuringMachine::step() {

}

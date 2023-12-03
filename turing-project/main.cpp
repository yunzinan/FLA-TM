#include <iostream>
using namespace std;
int main(int argc, char* argv[]){
    
    if(argc == 1){
        return 1;
    }
    printf("%d arguments received:\n", argc);
    for(int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    cout<<"This is for testing"<<endl;
        int divisor = 0;

    if (divisor == 0) {
        // 输出错误信息到cerr
        cerr<< "Error: Division by zero!" << std::endl;
    } else {
        cout << "Nothing wrong!" << endl;
    }
    return 0;
}
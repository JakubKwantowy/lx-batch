#include <iostream>
#include <vector>

using namespace std;

/**
 * @author JakubKwantowy
 * @name LX-Batch
*/

vector<string> splitStr(string input, char delim){
    string temp = "";
    vector<string> out = {};

    for(int i=0;i<input.length();i++){
        if(input[i] == delim){
            out.push_back(temp);
        }else{
            temp += input[i];
        }
    }
    out.push_back(temp);
    return out;
}

int main(int argc, char *argv[]){
    //cout << argv[1] << '\n';
    cout << "LX-Batch by JakubKwantowy\n";
    return 0;
}

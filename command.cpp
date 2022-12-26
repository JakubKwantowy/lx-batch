#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/utsname.h>
#include <algorithm>
#include <sstream>
#include "conio.h"

using namespace std;

/**
 * @author JakubKwantowy
 * @name LX-Batch
*/

string prompt = "$P>";
bool running = true;

int tcol = 10;
int bcol = 0;

int hexCharToInt(char input){
    stringstream ss;
    ss << input;
    int out;
    ss >> hex >> out;
    return out;
}

string toLower(string input){
    /**
     * Converts inputed string to Lowercase (Duh!) 
    */
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

string getPrompt(string input){
    /**
     * Gets current prompt from string (one input, one output. Everything should be clear.)
    */

    string out = "";
    bool escape = false;
    time_t *currtime_t;
    struct tm *currtm;

    for(int i=0;i<input.length();i++){
        if(escape){
            switch(input[i]){
                default:
                    out += '$';
                    out += input[i];
                break;
                case 'A':
                    out += '&';
                break;
                case 'B':
                    out += '|';
                break;
                case 'C':
                    out += '(';
                break;
                case 'D':
                    time(currtime_t);
                    currtm = localtime(currtime_t);
                    out += to_string(currtm->tm_mon + 1) + "/" + to_string(currtm->tm_mday) + "/" + to_string(currtm->tm_year + 1900);
                break;
                case 'E':
                    out += 0x1b;
                break;
                case 'F':
                    out += ')';
                break;
                case 'G':
                    out += '>';
                break;
                case 'H':
                    out += '\b';
                break;
                case 'L':
                    out += '<';
                break;
                case 'M':
                case 'N':
                case 'P':
                    out += get_current_dir_name();
                break;
                case 'Q':
                    out += '=';
                break;
                case 'S':
                    out += ' ';
                break;
                case 'T':
                    time(currtime_t);
                    currtm = localtime(currtime_t);
                    out += to_string(currtm->tm_hour) + ":" + to_string(currtm->tm_min) + ":" + to_string(currtm->tm_sec);
                break;
                case 'V':
                    struct utsname *osver;
                    uname(osver);
                    out += string(osver->sysname) + " " + string(osver->release);
                break;
                case '_':
                    out += '\n';
                break;
            }
            escape = false;
        }else{
            if(input[i] == '$') escape = true;
            else out += input[i];
        }
    }

    return out;

   /*
   $A - Ampersand & 
   $B - Pipe |
   $C - Opening Parenthesis (
   $D - Current Date
   $E - Escape Char 0x1B
   $F - Closing Parenthesis )
   $G - Greater Than >
   $H - Backspace \b
   $L - Less Than <
   $M - Name for Remote Drives
   $N - Current Drive
   $P - $N but with Path
   $Q - Equal to =
   $S - Space  
   $T - Current Time
   $V - OS Name + Release
   $_ - \n
   $$ - Dollar Sign $
   */
}

vector<string> splitStr(string input, char delim){
    string temp = "";
    vector<string> out = {};

    /**
     * Splits a String with a Delimeter 
     *
     * @param input String to be Split with the Delimeter
     * @param delim The Delimeter the string is going to be split with
     * 
     * @returns A vector that contains the String split up by the Delimeter
    */

    for(int i=0;i<input.length();i++){
        if(input[i] == delim){
            out.push_back(temp);
            temp = "";
        }else{
            temp += input[i];
        }
    }
    out.push_back(temp);
    return out;
}

string joinStr(vector<string> input, char delim, int skip){
    /**
     * Opposite of splitStr (joins a string instead of splitting)
     * 
     * @param skip Sets value to start at (0 on default)
    */

    string out = "";

    for(int i=skip;i<input.size();i++){
        out += input[i];
        if(i < input.size() - 1) out += delim;
    }

    return out;
}

string joinStr(vector<string> input, char delim){
    return joinStr(input, delim, 0);
}

int main(int argc, char *argv[]){
    //cout << argv[1] << '\n';
    textcolor(tcol + 1);
    textbackground(bcol);
    cout << "LX-Batch by JakubKwantowy\n\n";

    string userinp;
    vector<string> split_userinp = {};

    while(running){
        textcolor(tcol - 1);
        cout << getPrompt(prompt);
        textcolor(tcol);
        getline(cin, userinp);
        split_userinp = splitStr(userinp, ' ');

        if(userinp[0] == '.') system(userinp.c_str());
        else if(!toLower(split_userinp[0]).compare("exit")) running = false;
        else if(!toLower(split_userinp[0]).compare("cls")) clrscr();
        else if(!toLower(split_userinp[0]).compare("color")){
            tcol = hexCharToInt(split_userinp[1][1]);
            bcol = hexCharToInt(split_userinp[1][0]);
            textbackground(bcol);
            textcolor(tcol);
        }else if(!toLower(split_userinp[0]).compare("echo")){
            int skip = 1;
            if(split_userinp.size() > 1) {
                if(!split_userinp[1].compare("/n")) skip++;
                if(split_userinp.size() > 2 || skip < 2) cout << joinStr(split_userinp, ' ', skip);
            }
            if(skip < 2) cout << '\n';
        }
        else cout << "Incorrect Command: " << userinp << '\n';
    }

    cout << '\n';
    return 0;
}

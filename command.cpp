#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/utsname.h>
#include <algorithm>
#include <sstream>
#include <dirent.h>
#include "conio.h"

using namespace std;

/**
 * @author JakubKwantowy
 * @name LX-Batch
*/

string prompt = "$P>";
bool running = true;
string systemname = "LX-Batch by JakubKwantowy Beta";

int tcol = 7;
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
                    out += systemname;
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

string glueStr(vector<string> input, char delim, int offset){
    /**
     * Glues a String with a Delimeter 
     *
     * @param input String to be Glued with the Delimeter
     * @param delim The Delimeter the string is going to be glued with
     * @param offset The element to start glueing from, default is 0
     * 
     * @returns A string that contains the Vector glued up by the Delimeter
    */

    string out = "";
    for(int i=offset;i<input.size();i++) {
        out += input[i];
        if(i<(input.size()-1)) out += delim;
    } 
    return out;
}

string glueStr(vector<string> input, char delim){
    return glueStr(input, delim, 0);
}

int main(int argc, char *argv[]){
    //cout << argv[1] << '\n';

    textcolor(9);
    textbackground(bcol);
    cout << systemname << '\n' << '\n';

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
            if(split_userinp[1].length() > 1) {
                tcol = hexCharToInt(split_userinp[1][1]);
                bcol = hexCharToInt(split_userinp[1][0]);
            } else tcol = hexCharToInt(split_userinp[1][0]);
            textbackground(bcol);
            textcolor(tcol);
        }else if(!toLower(split_userinp[0]).compare("ver")){
            cout << systemname << '\n';
        }else if(!toLower(split_userinp[0]).compare("cd")){
            chdir(glueStr(split_userinp, ' ', 1).c_str());
        }else if(!toLower(split_userinp[0]).compare("dir")){
            
        }
        else cout << "Incorrect Command: " << userinp << '\n';
    }

    cout << '\n';
    return 0;
}

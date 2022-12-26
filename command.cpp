#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <unistd.h>
#include <sys/utsname.h>
#include "conio.h"

using namespace std;

/**
 * @author JakubKwantowy
 * @name LX-Batch
*/

string prompt = "$P>";
bool running = true;

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

int main(int argc, char *argv[]){
    //cout << argv[1] << '\n';
    cout << "LX-Batch by JakubKwantowy\n";

    string userinp;
    vector<string> split_userinp = {};

    while(running){
        cout << prompt;
        getline(cin, userinp);
        split_userinp = splitStr(userinp, ' ');

        cout << split_userinp[0];

        running = false;
    }

    cout << '\n';
    return 0;
}

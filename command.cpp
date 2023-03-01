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

//using namespace std;

/**
 * @author JakubKwantowy
 * @name LX-Batch
*/

std::string prompt = "$P>";
bool running = true;
std::string systemname = "LX-Batch by JakubKwantowy Beta";

std::vector<std::vector<std::string>> env = {
    {"%lxbatch%", "beta"}
};

int tcol = 10;
int bcol = 0;

int hexCharToInt(char input){
    std::stringstream ss;
    ss << input;
    int out;
    ss >> std::hex >> out;
    return out;
}

std::string toLower(std::string input){
    /**
     * Converts inputed string to Lowercase (Duh!) 
    */
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

std::string getPrompt(std::string input){
    /**
     * Gets current prompt from string (one input, one output. Everything should be clear.)
    */

    std::string out = "";
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
                    out += std::to_string(currtm->tm_mon + 1) + "/" + std::to_string(currtm->tm_mday) + "/" + std::to_string(currtm->tm_year + 1900);
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
                    out += std::to_string(currtm->tm_hour) + ":" + std::to_string(currtm->tm_min) + ":" + std::to_string(currtm->tm_sec);
                break;
                case 'V':
                    struct utsname *osver;
                    uname(osver);
                    out += std::string(osver->sysname) + " " + std::string(osver->release);
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

std::vector<std::string> splitStr(std::string input, char delim){
    std::string temp = "";
    std::vector<std::string> out = {};

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

std::string glueStr(std::vector<std::string> input, char delim, int offset){
    /**
     * Glues a String with a Delimeter 
     *
     * @param input String to be Glued with the Delimeter
     * @param delim The Delimeter the string is going to be glued with
     * @param offset The element to start glueing from, default is 0
     * 
     * @returns A string that contains the Vector glued up by the Delimeter
    */

    std::string out = "";
    for(int i=offset;i<input.size();i++) {
        out += input[i];
        if(i<(input.size()-1)) out += delim;
    } 
    return out;
}

std::string glueStr(std::vector<std::string> input, char delim){
    return glueStr(input, delim, 0);
}

std::string findElement(std::vector<std::vector<std::string>> dict, std::string toFind){
    for(std::vector<std::string> pair : dict){
        if(pair[0] == toFind) return pair[1];
    }
    return NULL;
}

int elementId(std::vector<std::vector<std::string>> dict, std::string toFind){
    for(int i=0;i<dict.size();i++){
        if(dict[i][0] == toFind) return i;
    }
    return NULL;
}

bool keyExists(std::vector<std::vector<std::string>> dict, std::string key){
    for(std::vector<std::string> pair : dict){
        if(pair[0] == key) return true;
    }
    return false;
}

int main(int argc, char *argv[]){
    //cout << argv[1] << '\n';
    env.push_back({"%userprofile%", std::string(getenv("HOME"))});

    textcolor(9);
    textbackground(bcol);
    std::cout << systemname << '\n' << '\n';

    std::string userinp;
    std::vector<std::string> split_userinp = {};

    //std::cout << findElement(env, "%userprofile%");

    while(running){
        textcolor(tcol - 1);
        std::cout << getPrompt(prompt);
        textcolor(tcol);
        getline(std::cin, userinp);
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
        }else if(!toLower(split_userinp[0]).compare("echo")){
            int skip = 1;
            if(split_userinp.size() > 1) {
                if(!split_userinp[1].compare("-n")) skip++;
                if(split_userinp.size() > 2 || skip < 2) std::cout << glueStr(split_userinp, ' ', skip);
            }
            if(skip < 2) std::cout << '\n';
        }else if(!toLower(split_userinp[0]).compare("ver")){
            std::cout << systemname << '\n';
        }else if(!toLower(split_userinp[0]).compare("cd")){
            chdir(glueStr(split_userinp, ' ', 1).c_str());
        }else if(!toLower(split_userinp[0]).compare("dir")){
            DIR *dir; struct dirent *diread; // Compatibility 100
            std::vector<struct dirent *> files;
            int filec = 0, dirc = 0;
            std::string path = get_current_dir_name();
            if(split_userinp.size() > 1) {
                if(split_userinp[1][0] == '/') path = glueStr(split_userinp, ' ', 1);
                else path += '/' + glueStr(split_userinp, ' ', 1);
            }
            if ((dir = opendir(path.c_str())) != nullptr) {
                while ((diread = readdir(dir)) != nullptr) {
                    files.push_back(diread);
                }
                closedir (dir);
                std::cout << " Directory of " << path << ".\n\n";
                for(auto file : files){
                    if(file->d_name[0] == '.') continue;
                    if(file->d_type==DT_DIR){
                        std::cout << "<DIR> ";
                        dirc++;
                    }
                    else {
                        std::cout << "      ";
                        filec++;
                    }
                    std::cout << file->d_name;
                    std::cout << '\n';
                }
                std::cout << "    " << filec << " File(s)\n";
                std::cout << "    " << dirc << " Dir(s)\n";
            }
        }else if(!toLower(split_userinp[0]).compare("set")){
            if(split_userinp.size() < 2){
                for(int i=0;i<env.size();i++){
                    textcolor(tcol - 1);
                    std::cout << env[i][0]; 
                    textcolor(tcol);
                    std::cout << '=' << env[i][1] << '\n';
                }
                continue;
            }
            if(split_userinp[1] == "-p"){
                std::string envinp;
                std::vector<std::string> parts = splitStr(glueStr(split_userinp, ' ', 2), '=');
                textcolor(tcol - 1);
                std::cout << glueStr(parts, '=', 1);
                textcolor(tcol);
                getline(std::cin, envinp);
                int e;
                if((e = elementId(env, "%"+parts[0]+"%")) != NULL) env[e][1] = envinp;
                else env.push_back({"%"+parts[0]+"%", envinp});
                continue;
            }
            std::vector<std::string> parts = splitStr(glueStr(split_userinp, ' ', 1), '=');
            if(parts.size() > 1){
                int e;
                if((e = elementId(env, "%"+parts[0]+"%")) != NULL) env[e][1] = glueStr(parts, '=', 1);
                else env.push_back({"%"+parts[0]+"%", glueStr(parts, '=', 1)});
            }else{
                textcolor(tcol - 1);
                std::cout << split_userinp[1]; 
                textcolor(tcol);
                std::cout << '=' << findElement(env, split_userinp[1]) << '\n';
            }
        }else std::cout << "Incorrect Command: " << userinp << '\n';
    }

    std::cout << '\n';
    return 0;
}

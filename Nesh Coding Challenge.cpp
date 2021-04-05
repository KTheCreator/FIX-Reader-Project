// Nesh Coding Challenge.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include<ctime>
#include<iomanip>
#include<sstream>

using namespace std;

vector<string> getInputFromFile(string filename) {
    vector<string> testStr;
    ifstream file(filename);
    string content;
    while (getline(file, content)) {
        testStr.push_back(content);
    }
    return testStr;
}

vector<string> tokenise(string uInput) {
    /*
    * Purpose: Takes a given sequence, separates the sequence into a vector and returns
    */
    //if (uInput[uInput.size() - 1] != '.') uInput.push_back('.');//Ensures that the last word is included by adding a full stop if needs be.

    string word = "";
    vector<string> tokens;
    //Iterates through the characters and pushes them into a string stack. The space or fullstop indicates the end of the word. The process is repeated till the end.
    for (auto& i : uInput) {
        if (i == '') { tokens.push_back(word); word = ""; }
        else word.push_back(i);
    }
    return tokens;
}

bool gettingEssentialTags(string x) {
    vector<string>validTags = {"31","32","54","55"};
    for (int i = 0; i < validTags.size(); i++) {
        if (validTags[i] == x) return true;
    }
    return false;
}

bool isNumber(string x) {
    /*
    * Purpose: Checks if the given string is a number.
    */
    int decimalPointApperance = 0;//Only for tag 31
    if (x[2] == '=') {
        for (int i = 3; i < x.length(); i++) {
            if (x[i] == '.') {
                decimalPointApperance++;
            }
            else if (!isdigit(x[i])) return false;
        }
        if (x.substr(0, 2) == "31") {
            if (decimalPointApperance != 1) return false;
            else return true;
        }
        else if (x.substr(0, 2) != "31") {
            if (decimalPointApperance > 0) return false;
            else return true;
        }
        else return true;
    }
    else return false;
    
}

bool isSymbols(string x) {
    if (x[2] == '=') {
        for (int i = 3; i < x.length(); i++) {
            if (isdigit(x[i])) return false;
            if (!isalpha(x[i])) return false;
        }
        return true;
    }
    else return false;
}

bool checkingTags(string currTag) {
    /*
    - Tag 31: Must be numerical and a none 0 digit
    */
    if (currTag.substr(0, 2) == "31") {
        if (currTag[3] == '0') return false;//Checks if the value is 0.
        if (!isNumber(currTag)) return false;//Checks if there is a decimal.
        
    }

    /*
    -Tag 32: Has to be positive and non zero number
    */
    else if (currTag.substr(0, 2) == "32") {
        if (currTag[3] == '-' || currTag [3] == '0') return false;//Checks that its not 0 or a negative number
        if (!isNumber(currTag)) return false;//Checks that it is numerical
    }

    /*
    -Tag 54: Can only Be 1 or 2
    */
    else if (currTag.substr(0, 2) == "54") {
        if (currTag[3] != '2' && currTag[3]!='1') return false;
        if (!isNumber(currTag)) return false;
    }

    /*
    -Tag 55:Checks if it is alphanumerical non-numerical
    */
    else if (currTag.substr(0, 2) == "55") {
        if (!isSymbols(currTag)) return false;
    }
    return true;
}

bool isMessageValid(vector<string>eTags) {
    if (eTags.size() == 4) {
        for (int i = 0; i < eTags.size(); i++) {
            if (checkingTags(eTags[i])) continue;
            else { return false; }
        }
        return true;
    }
    else { return false; }
}

int boughtOrSold(string tag54) {
    if (tag54[3] == '1') return 1;
    else if (tag54[3] == '2') return 2;
}

bool presentInList(vector<string>list,string target) {
    if (list.size() != 0) {
        for (int i = 0; i < list.size(); i++) {
            if (list[i] == target) return true;
        }
        return false;
    }
    return false;
}

string padNumbers(int target) {
    ostringstream temp;
    temp << internal << setfill('0') << setw(2) << target;
    return temp.str();
}

void writeToFile(int boughtSize, int soldSize,int bothSize) {
    //Getting the current date for file title
    struct tm newtime;
    time_t t= time(0);
    localtime_s(&newtime,&t);
    string year = to_string((newtime.tm_year)+1900);
  
    string month = padNumbers((newtime.tm_mon) + 1);
    string day = padNumbers((newtime.tm_mday));
    

    string fileName = year + "-" + month + "-" + day + "_summary.txt";
    ofstream targetFile(fileName);
    targetFile << "Bought: " << boughtSize << endl;
    targetFile << "Sold: " << soldSize << endl;
    targetFile << "Both: " << bothSize << endl;
    targetFile.close();

}



void mainProcess(string fileName) {
    vector<string>Bought;
    vector<string>Sold;
    vector<string>Both;
    int failedLines = 0;
    

    vector<string>linesFromFile = getInputFromFile(fileName);
    
    for (int i = 0; i < linesFromFile.size(); i++) {
        vector<string>currFIXLine = tokenise(linesFromFile[i]);
        vector<string>essentialTags;
        for (int i = 0; i < currFIXLine.size(); i++) {
            if (gettingEssentialTags(currFIXLine[i].substr(0, 2)))
                essentialTags.push_back(currFIXLine[i]);
        }
        if (!isMessageValid(essentialTags)) {
            failedLines++; continue;
        }
        string tag54 = "";
        string tag55 = "";
        for (int i = 0; i < essentialTags.size(); i++) {
            if (essentialTags[i].substr(0, 2) == "54") {
                tag54 = essentialTags[i];
            }
            else if (essentialTags[i].substr(0, 2) == "55") {
                string temp = "";
                for (int j = 3; j < essentialTags[i].size(); j++) {
                    temp.push_back(essentialTags[i][j]);
                }
                tag55 = temp;
            }

        }

        int bOSResult = boughtOrSold(tag54);
        if (bOSResult == 1) {
            if (!presentInList(Bought, tag55)) {
                Bought.push_back(tag55);
            }
            else continue;
        }
        else {
            if (!presentInList(Sold, tag55)) Sold.push_back(tag55);
            else continue;
        }
    }
    
    if (Bought.size() <= Sold.size()) {
        for (int i = 0; i < Bought.size(); i++) {
            if (presentInList(Sold, Bought[i])) Both.push_back(Bought[i]);
        }
    }
    else {
        for (int i = 0; i < Sold.size(); i++) {
            if (presentInList(Bought, Sold[i])) Both.push_back(Sold[i]);
        }
    }

    writeToFile(Bought.size(),Sold.size(),Both.size());

    cout << "Bought: " << Bought.size() << endl;
    cout << "Sold: " << Sold.size() << endl;
    cout << "Both: " << Both.size() << endl;
    cout << "Failed Messages: " << failedLines << endl;
    cout << "Total: " << linesFromFile.size()<<endl;

}
int main()
{
    string userChoice;
    cout<< "What File Am I Reading? " << endl;
    cin >> userChoice;
    mainProcess(userChoice);
}
#ifndef BOTTLES_H
#define BOTTLES_H

#define MAX_BOTTLE_CAPACITY  4

#include <string>
#include <sstream>
#include <vector>

#include <bits/stdc++.h>

using namespace std;

class Bottles
{
public:
    Bottles();
    Bottles(string b[], int numberOfBottles);

    string toString () const;

    bool isSolved();
    static int isValidMove(string sourceBottle, string destinationBottle);
    bool makeMove(Bottles &n, string sourceBottle, string destinationBottle, int i, int j);
    vector <Bottles *> expand();

    int getDepth();

    void setActionName(string s) {actionName=s;}
    string getActionName()const {return actionName;}
    void setPrevious (Bottles *p) {prev=p;}
    Bottles *getPrevious()const {return prev;}

    string getKey()
    {
        vector<string> bottles2=bottles;

        string finalString;
        sort(bottles2.begin(), bottles2.end());

        for (const auto& bottle : bottles2)
            finalString+=(bottle+";");

        return finalString;
    }
    friend ostream & operator<<(ostream &, const Bottles &);
    void printStates() const;
private:
    vector<string> bottles;
    string actionName;
    Bottles *prev;
};

#endif //BOTTLES_H

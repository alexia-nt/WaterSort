#include "Bottles.h"

#include <iostream>
using namespace std;

Bottles::Bottles()
{
    actionName = "";
    prev = nullptr;
}

Bottles::Bottles(string b[], int numberOfBottles)
{
    for (int i=0; i<numberOfBottles; i++)
        bottles.push_back(b[i]);

    actionName = "";
    prev = nullptr;
}

string Bottles::toString () const
{
    stringstream ot;

    for(const auto& bottle : bottles){
        if(bottle.size()==MAX_BOTTLE_CAPACITY){ //if the bottle is full
            ot<<bottle<<';';
        }
        else if(!bottle.empty() && bottle.size()<MAX_BOTTLE_CAPACITY){ //if the bottle is not full but also not empty
            ot<<bottle;
            for(int i=0; i<MAX_BOTTLE_CAPACITY-bottle.size(); i++){
                ot<<"_";
            }
            ot<<";";
        }
        else{ //if the bottle is empty
            ot<<"____;";
        }
    }
    return ot.str();
}

bool Bottles::isSolved()
{
    for (auto bottle : bottles) {
        if (bottle.empty()) //if the bottle is empty
            continue;
        else if (bottle.size() < MAX_BOTTLE_CAPACITY) //if the bottle is not full
            return false;
        else if (count(bottle.begin(),
                       bottle.end(),
                       bottle[0])
                 != MAX_BOTTLE_CAPACITY) //if the bottle is not same colored
            return false;
    }
    return true;
}

int Bottles::isValidMove(string sourceBottle, string destinationBottle)
{
    //Can't pour from an empty bottle or to a full bottle
    if (sourceBottle.empty() || destinationBottle.size() == MAX_BOTTLE_CAPACITY)
        return 0;

    int colorFreqs = count(sourceBottle.begin(),
                           sourceBottle.end(),
                           sourceBottle[0]);

    //If the source bottle is same colored and full, don't touch it
    if (colorFreqs == MAX_BOTTLE_CAPACITY)
        return 0;

    int k=1;
    char topColor = sourceBottle.back(); //source bottle is not empty
    int i=0;
    while(i < sourceBottle.size()-1){
        if(sourceBottle[sourceBottle.size() - (i+2)] == topColor){
            k++;
        }
        else{
            break;
        }
        i++;
    }
    //k ml of the same colored liquid at the top of the source bottle

    if (destinationBottle.empty()) {
        //If source bottle has only same colored liquid, don't touch it (to avoid loops)
        if (colorFreqs == sourceBottle.size())
            return 0;

        return k; //return k ml
    }
    //In any other case check the top colors of the two bottles
    //and if the k ml fit in the destination bottle
    if(sourceBottle[sourceBottle.size() - 1] == destinationBottle[destinationBottle.size() - 1]){
        if(k <= MAX_BOTTLE_CAPACITY - destinationBottle.size()){ //if k ml fit in the destination bottle
            return k;
        }
        else{
            return MAX_BOTTLE_CAPACITY - destinationBottle.size(); //ml that fit in the destination bottle
        }
    }
    else{
        return 0;
    }
}

bool Bottles::makeMove(Bottles &n, string sourceBottle, string destinationBottle, int i, int j)
{
    int k = isValidMove(sourceBottle, destinationBottle);
    if(k>0){
        n=*this;

        for (int l=0; l < k; l++){
            n.bottles[j].push_back(n.bottles[i].back());
            n.bottles[i].pop_back();
        }

        string str_i = to_string(i+1);
        string str_j = to_string(j+1);

        //adding space to 1-digit numbers to align the output
        if(i<9)
            str_i=" "+str_i;
        if(j<9)
            str_j=" "+str_j;

        n.setActionName("Pour "+to_string(k)+" ml from "+str_i+" to "+str_j+";");
        n.setPrevious(this);
        return true;
    }
    return false;
}

vector <Bottles *> Bottles::expand()
{
    vector <Bottles *> children;
    Bottles *child;

    for (int i = 0; i < bottles.size(); i++) {
        //Iterate over all the bottles
        string sourceBottle = bottles[i];
        for (int j = 0; j < bottles.size(); j++) {
            if (i == j)
                continue;
            string destinationBottle = bottles[j];

            child = new Bottles(*this);
            if(makeMove(*child,sourceBottle,destinationBottle,i,j)) {
                children.push_back(child);
            }
            else {
                delete child;
            }
        }
    }
    return children;
}

int Bottles::getDepth()
{
    int counter =0;
    Bottles *p = this;
    while (p->prev!=nullptr)
    {
        p=p->prev;
        counter++;
    }
    return counter;
}

ostream & operator<<(ostream &stream, const Bottles &a)
{
    string s; //s="";

    s=a.getActionName();
    Bottles *p=a.getPrevious();

    while(p!=nullptr)
    {
        s = p->getActionName()+"\n"+s;
        p = p->getPrevious();
    }
    stream<<s;
    return stream;
}

void Bottles::printStates() const{
    string s; //s="";

    s=getActionName()+"   "+toString();
    Bottles *p=getPrevious();

    while(p!=nullptr)
    {
        if(p->getPrevious()== nullptr)
            s="Starting from:             "+p->toString()+"\n"+s;
        else
            s = p->getActionName()+"   "+p->toString()+"\n"+s;

        p = p->getPrevious();
    }
    cout<<s;
}

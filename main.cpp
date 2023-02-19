#include <iostream>
#include <random>
#include <unordered_set>
#include <unordered_map>
#include <ctime>
#include "Bottles.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <chrono>

using namespace std;

Bottles *BFS(Bottles *initial,long long &examined,long long &mem)
{
    queue<Bottles *> agenda; //δημιουργώ το μέτωπο της αναζήτησης
    unordered_map <string,Bottles *> closed; //δημιουργώ το κλειστό σύνολο

    agenda.push(initial); //προσθέτω την αρχική κατάσταση στο μέτωπο
    examined=0;
    mem=1;
    while (agenda.size()>0) //όσο το μέτωπο δεν είναι άδειο
    {
        if (agenda.size() + closed.size() > mem)
            mem = agenda.size() + closed.size();

        Bottles *s = agenda.front(); //παίρνω την πρώτη κατάσταση από το μέτωπο
        agenda.pop(); //αφαιρώ την πρώτη κατάσταση από το μέτωπο

        if (closed.count(s->getKey())==0) //αν η τρέχουσα κατάσταση δεν ανήκει στο κλειστό σύνολο
        {
            examined++;
            if (s->isSolved()) //αν η τρέχουσα κατάσταση είναι η τελική
                return s; //επιστρέφω τη λύση του προβλήματος

            pair<string,Bottles *> k (s->getKey(),s);
            closed.insert(k); //προσθέτω την τρέχουσα κατάσταση στο κλειστό σύνολο

            vector<Bottles *> children =s->expand(); //βρίσκω τα παιδιά της τρέχουσας κατάστασης
            for(unsigned int i=0;i<children.size();i++) //για κάθε παιδί της τρέχουσας κατάστασης
                if (closed.count(children[i]->getKey())==0) //αν το παιδί της τρέχουσας κατάστασης δεν ανήκει στο κλειστό σύνολο
                    agenda.push(children[i]); //προσθέτω το παιδί στο μέτωπο της αναζήτησης
        }
    }
    return nullptr;
}

string random_string(int k){
    string s="";

    for(int i=0; i<k; i++){
        for(int j=0; j<4; j++){
            s+=char(i+97);
        }
    }

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(s.begin(), s.end(), default_random_engine(seed));

    return s;
}

string *random_bottles(int numberOfBottles){
    string str = random_string(numberOfBottles-2);

    string *b = new string[numberOfBottles];

    for(int i=0; i<numberOfBottles; i++) {
        b[i]="";
    }

    for(int i=0; i<numberOfBottles-2; i++) {
        for(int j=0; j<4; j++){
            b[i].push_back(str[4*i+j]);
        }
    }
    return b;
}

int main()
{
    clock_t start = clock();

    long long mem,examined;

    srand(time(NULL));
    int numberOfBottles = rand()%17+4; // Including 2 empty bottles
    cout<<"Number of bottles: "<<numberOfBottles<<endl;

    string *bottles=random_bottles(numberOfBottles);

    Bottles startState(bottles, numberOfBottles);

    Bottles *initialState = &startState;
    Bottles *bsol; //=initial;

    cout<<endl<<"BFS: ";
    bsol=BFS(initialState,examined,mem);
    if (bsol!=nullptr)
    {
        cout<<"depth = "<<bsol->getDepth()<<", Mem: "<<mem<<", Examined: "<<examined<<endl;
        cout<<endl;
        bsol->printStates();
    }
    else
        cout<<"Problem unsolvable"<<endl;
    cout<<endl;

    clock_t stop = clock();
    double elapsed = (double)(stop-start)/CLOCKS_PER_SEC;
    cout<<"Time elapsed: "<<elapsed<<" seconds"<<endl;

    return 0;
}

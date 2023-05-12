#include<bits/stdc++.h>
using namespace std;

/* global variables */
const int GENERATIONS = 100;             
const int POP_SIZE = 100;                   // population size
const string GOAL = "Zeumnska gimnazija";   // goal string
const int LEN = (int)GOAL.size();           // length of goal string
vector<string> POPULATION(POP_SIZE);

/* utility functions */
string getRandomString() {
    string res = "";

    char rand_char;
    for (int i = 0; i < LEN; i++) {
        rand_char = (char)((rand() % 94) + 33);
        res += rand_char;
    }

    return res;
}

/* main functions */

int fitness(string Organism) {
    /* calculates fitness of an organism */    

    int err = 0;
    for (int i = 0; i < LEN; i++) {
        err += abs(GOAL[i] - Organism[i]);
    }
    return err;
}

string selectParent() {
    /* selects parent using 
    roulette wheen algorithm */

    return POPULATION[0];
}

string mateParents(string p1, string p2) {
    /* */
    string ch;
    for (int i = 0; i < LEN; i++) {
        if ((rand() % 10) < 5) ch += p1[i];
        else ch += p2[i];
    }
    return ch;
}

void mutate() {}

void mate() {
    /* constructs new generation */

    for (int i = 0; i < (POP_SIZE / 2); i++) {
       string p1 = selectParent();
       string p2;
       do {
            p2 = selectParent();
       }
       while(p1 == p2);

        string ch1 = mateParents(p1, p2);
        string ch2 = mateParents(p1, p2);

        // add offspring to population
    }
}

void initializePopulation() {
    /* initializes random population */
    for (int i = 0; i < POP_SIZE; i++) {
        POPULATION[i] = getRandomString();
    }
}

int main() {

    initializePopulation();

    for (int i = 0; i < GENERATIONS; i++) {
        mate();
        mutate();
    }  
    
    return 0;
}
#include<bits/stdc++.h>
#include "organism.h"
using namespace std;

// target function
double f(double x) {
    return 1.0;
}

// global parameters - maybe put into a separate file?
const int POP_SIZE = 100;
const double A, B;  // interval on which the maximum is searched
vector<Organism> POPULATION;

// fucntions
double cost(const Organism &o) {
    // calculate how good an organism is
    return 1.0;
}

int createRandomDNA() {
    /* returns random DNA, used for initializing population */

    int DNA = 0;
    for (int i = 31; i >= 0; i--) {
        if (rand() % 2 == 1) {
            // i-th bit should be 1
            DNA |= (1 << i);
        }
        // else bit stays 0
    }

    return DNA;
}

void initializePopulation() {
    /* creates initial population of size POP_SIZE */

    for (int i = 0; i < POP_SIZE; i++) {
        Organism o;
        o.DNA = createRandomDNA();

        POPULATION.push_back(o);
    }
}


int main() {

    initializePopulation(); 

    return 0;
}
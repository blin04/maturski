#include<bits/stdc++.h>
#include "organism.h"
using namespace std;

// target function, defined on [A, B]
double f(double x) {
    return (2*sin(x)*sin(x)) / x;
}

// global parameters - maybe put into a separate file?
const int POP_SIZE = 100;
const double A = -10.0;     // interval start
const double B = 10.0;      // interval end 
const int N = 32;           // length of chromosome
vector<Organism> POPULATION;

// fucntions
double cost(const Organism &o) {
    // calculate how good an organism is
    return 1.0;
}

double getEncodedNumber(Organism o)
{
    double x = (o.getValue()) / ((1 << (N - 1)) - 1.0);
    return A + x*(B - A);
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

    cout << getEncodedNumber(POPULATION[0]) << "\n";

    return 0;
}
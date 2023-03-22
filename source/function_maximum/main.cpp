#include<bits/stdc++.h>
#include "organism.h"
using namespace std;

// target function, defined on [A, B]
double f(double x) {
    return (2*sin(x)*sin(x)) / x;
}

// global parameters - maybe put into a separate file?
const int POP_SIZE = 100;
const int GENRATIONS = 1000;
const double A = -10.0;     // interval start
const double B = 10.0;      // interval end 
const double CONST = 20.0;   // constant used so that f(x) is not negative
const int N = 32;           // length of chromosome
vector<Organism> POPULATION;

// fucntions
double getEncodedNumber(Organism &o)
{
    /* returns real number that is encoded by some chromosome */

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

double fitness(Organism &o) {
    /* calculates fitness function of an organism 
       in other words, how good an organism is */

    double x = getEncodedNumber(o); 
    return f(x) + CONST;
}

double totalFitness() {
    /* calculates total fitness of the population */

    double total = 0.0;
    for (auto &o : POPULATION) {
        total += fitness(o);
    }
    return total;
}


int main() {

    initializePopulation(); 

    cout << setprecision(15) << totalFitness() << "\n";

    return 0;
}
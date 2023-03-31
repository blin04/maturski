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
const double CONST = 1.0;   // constant used so that f(x) is not negative 
const int N = 32;           // length of chromosome
vector<Organism> POPULATION, NEW_POPULATION;

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
    /* creates initial population of
     random organisms with size of POP_SIZE */

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

Organism selectParents() {
    /* selects a parent from current population
    using roulette wheel algorithm */

    
    // choose a random real number from 0 to total
    double r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / totalFitness());

    double sum = 0;
    Organism o;
    for (int i = 0; i < POP_SIZE; i++) {
        sum += fitness(POPULATION[i]);

        // stop when random number is between fitness of two organsims
        if (sum > r) {
            o = POPULATION[i];
            break;
        }
    }

    return o;
}

int mateParents(Organism &p1, Organism &p2) {
    /* performs mating algorithm and returns a child of given parents */
    return -1;
}

void createNewPopulation() {
    /* creates new generation by mating parents
     from current population */

    set<int> diff;

    Organism p1, p2;    // parents
    Organism ch1, ch2;  // children
    for(int i = 0; i < (POP_SIZE / 2); i++) {
        // selecting two parents
        p1 = selectParents();
        do {
            // assuring organism doesn't mate with itself
            p2 = selectParents();
        }
        while(p2 == p1); 

        diff.insert(p1.DNA);
        diff.insert(p2.DNA);
    }

    cout << "Number of different selected parents is " << diff.size() << "\n";

    return;
}


int main() {

    initializePopulation(); 

    createNewPopulation();

   /* for (int i = 0; i < GENRATIONS; i++) {
        createNewPopulation();
    } */

    return 0;
}
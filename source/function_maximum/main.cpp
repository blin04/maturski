#include<bits/stdc++.h>
#include "organism.h"
using namespace std;


// global parameters - maybe put into a separate file?
const int POP_SIZE = 100;
const int GENERATIONS = 100;
const double A = 0.0;       // interval start
const double B = 100.0;     // interval end 
double CONST = 0.0;         // constant used so that f(x) is not negative 
const int N = 32;           // length of chromosome
const int PM = 2;           // chance of mutation (in percentage)
vector<Organism> POPULATION, NEW_POPULATION;

// target function, defined on [A, B]

double f(double x) {
    return (pow(x, sin(x)));
}

// fucntions

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

void setConst() {
    /* sets value of CONST variable, which is used 
    so that fitness functions is positive for all organisms */

    for (Organism &o : POPULATION) {
        if (f(o.getEncodedNumber(A, B)) < 0)
            CONST = max(CONST, -f(o.getEncodedNumber(A, B)));
    } 
}

int randomMask() {
    /* generates random binary mask */
    int mask = 0;

    for (int i = 0; i < 32; i++) 
        if (rand() % 2) mask |= (1 << i);
    
    return mask;
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

    double x = o.getEncodedNumber(A, B); 
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

Organism mateParents(Organism &p1, Organism &p2, double p = 0.5) {
    /* performs p-uniform mating algorithm 
    and returns a child of given parents 
    (p is expected to be between 0 and 1) */

    Organism child;
    child.DNA = 0;
    for (int i = N - 1; i >= 0; i--) {
        if ((p1.DNA & (1 << i)) == (p2.DNA & (1 << i))) {
            // values of i-th bit are equal
            child.DNA |= (p1.DNA & (1 << i));
        }
        else {
            // values of i-th bit are different
            if (rand() % 10 < (p * 10)) {
                // choosing p1's bit
                child.DNA |= (p1.DNA & (1 << i));
            } 
            else {
                // choosing p2's bit
                child.DNA |= (p2.DNA & (1 << i));
            }
        }
    }

    return child;
}

void mutate() {
    /* mutate pm percentage of population */

    for (int i = 0; i < POP_SIZE; i++) {
        // organism has probability of PM/100 to be chosen

        if ((rand() % 100) < PM) {
            // mutation is performed as follows:
            //  * mask[i] == 1  ->  this bit stays as it was
            //  * mask[i] == 0  ->  this bit changes

            POPULATION[i].DNA ^= randomMask();
        }
    }

}

void mate() {
    /* performs mating on current population 
    and pushes children to new population */

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

        ch1 = mateParents(p1, p2);
        ch2 = mateParents(p1, p2);

        NEW_POPULATION.push_back(ch1);
        NEW_POPULATION.push_back(ch2);
    }

    POPULATION = NEW_POPULATION;
    return;
}


int main() {

    srand(time(NULL));

    initializePopulation(); 

    for (int i = 0; i < GENERATIONS; i++) {
        cout << i + 1 << ". generation\n";
        mate();
        mutate();
    } 

    Organism best = POPULATION[0];
    for (Organism &o : POPULATION) {
        if (fitness(o) > fitness(best)) {
            best = o;
        }
    }

    cout << "Found best solution: " << best.getEncodedNumber(A, B) << "\n";

    return 0;
}
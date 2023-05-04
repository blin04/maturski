#include<bits/stdc++.h>
#include "organism.h"
using namespace std;


// global parameters - maybe put into a separate file?
const int POP_SIZE = 500;
const int GENERATIONS = 100;
const double A = 0.0;       // interval start
const double B = 100.0;     // interval end 
const int N = 32;           // length of chromosome
const int M = 15;            // how many chromosomes are eliminated
const int PM = 2;           // chance of mutation (in percentage)
vector<Organism> POPULATION;

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
    return f(x);
}

double totalFitness() {
    /* calculates total fitness of the population */

    double total = 0.0;
    for (auto &o : POPULATION) {
        total += fitness(o);
    }
    return total;
}

double cost(Organism &o) {
    /* calculates 'how bad' an organism is 
    calculated by formula: max(fitness[0], ... , fitness[n]) - fitness[i] */

    double max_fitness = -1000;
    for (Organism &it : POPULATION) {
        if (fitness(it) > max_fitness)
            max_fitness = fitness(it);
    }

    return max_fitness - fitness(o);
}

double totalCost() {
    /* calculates total cost of the population */

    double total = 0.0;
    for (Organism &o : POPULATION) {
        total += cost(o);
    }
    return total;
}

bool compare(Organism &o1, Organism &o2) {
    /* function used to sort Organisms 
    based on their fitness values */

    return fitness(o1) < fitness(o2);
}

Organism selectParents() {
    /* selects a parent from current population
    using roulette wheel algorithm */

    
    // choose a random real number from 0 to total
    double r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / totalFitness());

    double sum = 0;
    Organism o;
    for (int i = 0; i < (int)POPULATION.size(); i++) {
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
    /* performs mating on remaining part of 
    population after elimination process*/

    Organism p1, p2, ch;
    for (int i = 0; i < M; i++) {
        p1 = selectParents();
        do {
            p2 = selectParents();
        }
        while(p1 == p2);

        // should check if ch is duplicate
        ch = mateParents(p1, p2);
        POPULATION.push_back(ch);
    }

}

void eliminate() {
    /* eliminate M organisms from population 
    using roulette wheel algorithm */

    double total_cost = totalCost();

    double r, sum = 0.0;
    for (int i = 0; i < M; i++) {
        r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / total_cost);

        sum = 0;
        for (auto it = POPULATION.begin(); it != POPULATION.end(); it++) {
            sum += cost(*it);

            // stop when random number is between cost of two organsims
            if (sum > r) {
                // eliminate this organism from population
                total_cost -= cost(*it);
                POPULATION.erase(it);
                break;
            }
        }
    }
}


int main() {

    initializePopulation(); 

    for (int i = 0; i < GENERATIONS; i++) {
        cout << i + 1 << ". generation\n";
        eliminate();
        mate();
        mutate();
    }

    Organism best = POPULATION[0];
    for (Organism &o : POPULATION) {
        if (fitness(o) > fitness(best)) 
            best = o;
    }

    cout << "Solution: x = " << best.getEncodedNumber(A, B) << ", F(X) = " << f(best.getEncodedNumber(A, B)) << "\n";

    return 0;
}
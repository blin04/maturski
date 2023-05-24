#include<iostream>
#include<vector>
#include<random>
#include<fstream>
#include<climits>
#include<time.h>
#include "organism.h"
using namespace std;


// global parameters - maybe put into a separate file?
const int POP_SIZE = 200;
const int GENERATIONS = 150;
double A = -2.0;            // interval start
double B = 6.0;             // interval end 
const int N = 32;           // length of chromosome
const int M = 15;           // how many chromosomes are eliminated
const int PM = 2;           // chance of mutation (in percentage)
double (*f)(double x);      // pointer to used function
vector<Organism> POPULATION;

/* test functions */

double f1(double x) {
    return pow(x, 5) - 5 * pow(x, 3) - (x * x) / 3 + 4 * x + 1;
}

double f2(double x) {
    return pow(x, sin(x));
}

double f3(double x) {
    return x * cos(tan(x));
}

double f4(double x) {
    return (sin(10 * M_PI * x*x) / x);
}

/* utility functions */

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

/* main functions */

void initializePopulation() {
    /* creates initial population of
     random organisms with size of POP_SIZE */

    for (int i = 0; i < POP_SIZE; i++) {
        Organism o;
        o.DNA = createRandomDNA();

        POPULATION.push_back(o);
    }
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
            int rand_pos = rand() % N;

            POPULATION[i].DNA ^= (1 << rand_pos);
        }
    }

}

void mate() {
    /* performs mating on remaining part of 
    population after elimination process*/

    Organism p1, p2, ch;
    while(POPULATION.size() != POP_SIZE) {
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

    srand(time(NULL));

    // general input
  /*  int fun; cout << "Izaberi funkciju (1-4): ";
    cin >> fun;
    cout << "Odredi interval [A - B]: \n";
    cout << "A: "; cin >> A;
    cout << "B: "; cin >> B; */

    f = f3;

    int start = time(NULL);

    initializePopulation(); 
    for (int i = 0; i < GENERATIONS; i++) {
        // cout << i + 1 << ". generation\n";
        eliminate();
        mate();
        mutate();
    }

    int end = time(NULL);

    Organism best = POPULATION[0];
    for (Organism &o : POPULATION) {
        if (fitness(o) > fitness(best)) 
            best = o;
    }

    // general output
    // cout << "Maksiumum f-je je u tacki X = " << best.getEncodedNumber(A, B) << "\n";
    // cout << "Vreme izvrsavanja: " << end - start << "s\n";

    // testing output
    cout << best.getEncodedNumber(A, B) << " ";
    cout << end - start << "\n";

    return 0;
}
#include<iostream>
#include<vector>
#include<random>
#include<climits>
#include<fstream>
#include<time.h>
#include<stdlib.h>
#include "organism.h"
using namespace std;


const int POP_SIZE = 200;
const int GENERATIONS = 150;
double A = -40.0;            // interval start
double B = 40.0;             // interval end 
double CONST = 0.0;         // constant used so that f(x) is not negative 
const int N = 32;           // length of chromosome
const int PM = 2;           // chance of mutation (in percentage)
double (*f)(double);        // pointer to used function
vector<Organism> POPULATION, NEW_POPULATION;

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
    return (sin(10 * M_PI * x*x)) / x;
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
            // randomly change bit

            int rand_pos = rand() % N;
            POPULATION[i].DNA ^= (1 << rand_pos);
        }
    }

}

void mate() {
    /* performs mating on current population 
    and pushes children to new population */
    
    NEW_POPULATION.clear();

    Organism p1, p2;    // parents
    Organism ch1, ch2;  // children
    for(int i = 0; i < (POP_SIZE / 2); i++) {
        // selecting two parents
        p1 = selectParents();
        p2 = selectParents();

        ch1 = mateParents(p1, p2);
        ch2 = mateParents(p1, p2);

        NEW_POPULATION.push_back(ch1);
        NEW_POPULATION.push_back(ch2);
    }

    POPULATION = NEW_POPULATION;
    return;
}

void write(int gen) {
    /* writes current population into a file */

    string file_name = "results/gen";

    if (gen == 100) file_name += "100";
    else {
        file_name += ('0' + (gen / 10));
        file_name += ('0' + (gen % 10)); 
    }

    ofstream file(file_name);

    for (Organism &o : POPULATION) {
        file << o.getEncodedNumber(A, B) << " " << f(o.getEncodedNumber(A, B)) << "\n";
    }
    file.close();

    return;
}


int main() {

    srand(time(NULL));

    // general input
	/*
    int fun; cout << "Izaberi funkciju (1-4): ";
    cin >> fun;
    cout << "Odredi interval [A - B]: \n";
    cout << "A: "; cin >> A;
    cout << "B: "; cin >> B;

    switch (fun)
    {
        case 1:
            f = f1; 
            break;
        case 2:
            f = f2; 
            break;
        case 3:
            f = f3; 
            break;
        case 4:
            f = f4; 
            break;
        
        default:
            throw runtime_error("Pogresan izbor funckije");
            break;
    } */

	f = f4;

    int start = time(NULL);

    initializePopulation(); 
	//write(0);
    for (int i = 0; i < GENERATIONS; i++) {
        // cout << i + 1 << ". generation\n";
        mate();
        mutate();

        //if (i < 10 || (i == 99) || (i == 59)) write(i + 1);
    } 

    int end = time(NULL);

    Organism best = POPULATION[0];
    for (Organism &o : POPULATION) {
        if (fitness(o) > fitness(best)) {
            best = o;
        }
    }

	// general output
     //cout << "Funkcija dostize maksimum u tacki X = " << best.getEncodedNumber(A, B) << " ";
     //cout << ", F(X) = " << f(best.getEncodedNumber(A, B)) << "\n";
     //cout << "Vreme izvrsavanja: " << end - start << "s\n";

    // output for getting results
    cout << best.getEncodedNumber(A, B) << "\n";
    //cout << end - start << "\n";

    return 0;
}

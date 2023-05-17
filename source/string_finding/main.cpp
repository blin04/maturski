#include<bits/stdc++.h>
using namespace std;

/* global variables */
const int GENERATIONS = 150;             
const int POP_SIZE = 1000;              // population size
string GOAL = "Programiranje";          // goal string
int LEN = (int)GOAL.size();             // length of goal string
const int PM = 3;
const double PE = 0.2;
vector<string> POPULATION, NEW_POPULATION;

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

int totalFitness() {
    /* calculates total fitness of the population */

    int total = 0;
    for (string &o : POPULATION) {
        total += fitness(o);
    }
    return total;
}

string selectParent() {
    /* selects parent using 
    roulette wheen algorithm */

    // TODO: preraditi, preko normalne raspodele mozda

    double r = static_cast<double>(rand() / static_cast<double>(RAND_MAX / totalFitness()));

    double sum = 0;
    string chosen;
    for (int i = 0; i < POP_SIZE; i++) {
        sum += fitness(POPULATION[i]);

        // stop when random number is between 
        if (sum > r) {
            chosen = POPULATION[i];
            break;
        }    
    }
    return chosen;
}

string mateParents(string p1, string p2) {
    /* */
    string ch;
    for (int i = 0; i < LEN; i++) {
        if (abs(p1[i] - GOAL[i]) <= abs(p2[i] - GOAL[i])) {
            ch += p1[i];
        }
        else ch += p2[i];
    }
    return ch;
}

void mutate() {
    /* mutates strings */

    for (int i = 0; i < POP_SIZE; i++) {
        if ((rand()) % 100 < PM) {
            int rand_ind = (rand()) % LEN;
            char rand_char = (char)((rand() % 94) + 32);

            POPULATION[i][rand_ind] = rand_char;
        }
    }
}

void mate() {
    /* constructs new generation */
    NEW_POPULATION.clear();

    /* elitism */
  /*  int elite_size = static_cast<int>(PE * static_cast<double>(POP_SIZE));
    if (elite_size % 2) ++elite_size; */

    while((int)NEW_POPULATION.size() != POP_SIZE) {
       string p1 = selectParent();
       string p2;
       do {
            p2 = selectParent();
       }
       while(p1 == p2);

        string ch1 = mateParents(p1, p2);
        string ch2 = mateParents(p1, p2);

        // add offspring to population
        NEW_POPULATION.push_back(ch1);
        NEW_POPULATION.push_back(ch2);
    }

    POPULATION = NEW_POPULATION;
}

void initializePopulation() {
    /* initializes random population */
    for (int i = 0; i < POP_SIZE; i++) {
        POPULATION.push_back(getRandomString());
    }
}

int main() {
    srand(time(NULL));

    cout << "Unesite string: ";
    cin >> GOAL;
    LEN = (int)GOAL.size();

    int start = time(NULL);

    initializePopulation();
    for (int i = 0; i < GENERATIONS; i++) {
//        cout << i + 1 << ". generation\n";
        mate();
        mutate();
    }  

    int end = time(NULL);


    pair<int, string> ans = {INT_MAX, ""};
    for (int i = 0; i < POP_SIZE; i++) {
        if (ans.first > fitness(POPULATION[i])) {
           ans.first = fitness(POPULATION[i]); 
           ans.second = POPULATION[i];
        }
    }
    
    cout << "Pronadjen je string: " << ans.second << "\n";
    cout << "Vreme izvrsavanja algoritma je: " << end - start << "s\n";
    return 0;
}
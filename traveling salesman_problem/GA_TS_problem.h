#ifndef GA_TS_PROBLEM
#define GA_TS_PROBLEM

#include <stdlib.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <iostream>

#define GENERATION_SIZE 10
#define GENERATIONS_AMOUNT 50
#define NEW_INDIVIDUALS_PERCENTAGE 30

using namespace std;

struct Gen
{
    Gen(unsigned int id_new = -1, int x_new = 0, int y_new = 0) {
        id = id_new;
        x = x_new;
        y = y_new;
    }
    friend bool operator== (const Gen& g1, const Gen& g2) {
        return g1.id == g2.id;
    }
    friend bool operator!= (const Gen& g1, const Gen& g2) {
        return (g1.id != g2.id || g1.x != g2.x || g1.y != g2.y);
    }

    unsigned int id;
    int x, y;
};
struct Chromosome
{
    friend bool operator== (const Chromosome& chr1, const Chromosome& chr2) {
        if (chr1.gens.size() != chr2.gens.size())
            return false;
        for (int i = 0; i < chr1.gens.size(); i++) {
            if (chr1.gens[i] != chr2.gens[i])
                return false;
        }
        return true;
    }

    vector <Gen> gens;
};
struct Individual
{
    Individual() {
        fitness_value = 0;
    }
    bool operator< (const Individual& ind) const {
        return fitness_value < ind.fitness_value;
    }

    vector <Chromosome> chromosomes;
    float fitness_value;
};


class GA_TS_problem
{
public:
    GA_TS_problem();

    void setNewData(vector <Gen>);

    void run();

    void generatePrimaryGeneration();
    void doSelection();
    void doCrossingOver();
    void doMutation();

    float countTwoGensFitness(Gen, Gen);
    void countIndividualFitness(Individual*);
    void countCurrentGenerationFitness();
    void doCrossingOverFillNonInherited(Chromosome*, int, int, int);
    void doMutationInChromosome(Chromosome*);

    void printCurrentGeneration();
    void printCurrentGenerationFitnessValue(int = GENERATION_SIZE);
private:
    unsigned int gens_amount;
    vector <Gen> initial_dataset;

    vector <vector <float> > fitness_arr;

    vector <Individual> current_generation;

};
#endif // GA_TS_problem

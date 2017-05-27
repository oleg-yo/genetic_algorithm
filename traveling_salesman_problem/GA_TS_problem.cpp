#include "GA_TS_problem.h"


GA_TS_problem::GA_TS_problem() {
    srand(time(NULL));
}

void GA_TS_problem::setNewData(vector <Gen> initial_dataset_new) {
    gens_amount = initial_dataset_new.size();
    initial_dataset = initial_dataset_new;

    fitness_arr.erase(fitness_arr.begin(), fitness_arr.end());
    for (int i = 0; i < gens_amount; i++) {
        fitness_arr.push_back(vector <float> (gens_amount));
        for (int j = 0; j < gens_amount; j++) {
            if (i == j)
                continue;
            fitness_arr[i][j] = countTwoGensFitness(initial_dataset[i], initial_dataset[j]);
        }
    }
}
void GA_TS_problem::run() {
    generatePrimaryGeneration();
    countCurrentGenerationFitness();

    int generations_amount_counter = 0;
    while (generations_amount_counter < GENERATIONS_AMOUNT) {
        doSelection();

        cout << "Generation " << generations_amount_counter << ":" << endl;
        printCurrentGenerationFitnessValue();
        //printCurrentGeneration();

        doCrossingOver();
        //doMutation();

        generations_amount_counter++;
    }
}
void GA_TS_problem::generatePrimaryGeneration() {
    current_generation.erase(current_generation.begin(), current_generation.end());
    for (int i = 0; i < GENERATION_SIZE; i++) {
        Chromosome new_chromosome;
        vector <Gen> initial_dataset_temp = initial_dataset;
        while (initial_dataset_temp.size() > 0) {
            int rand_index = rand() % initial_dataset_temp.size();
            new_chromosome.gens.push_back(initial_dataset_temp[rand_index]);
            initial_dataset_temp.erase(initial_dataset_temp.begin() + rand_index);
        }

        Individual new_individual;
        new_individual.chromosomes.push_back(new_chromosome);

        current_generation.push_back(new_individual);
    }
}
void GA_TS_problem::doSelection() {
    sort(current_generation.begin(), current_generation.end());
}
void GA_TS_problem::doCrossingOver() {
    int new_individuals_amount = NEW_INDIVIDUALS_PERCENTAGE / 100.0 * current_generation.size();
    new_individuals_amount -= new_individuals_amount % 2 == 1 ? 1 : 0;
    current_generation.erase(current_generation.end() - new_individuals_amount, current_generation.end());

    vector <Individual> new_individuals;
    for (int i = 0; i + 1 < new_individuals_amount; i+=2) {
        int first_point = rand() % gens_amount;
        int second_point;
        do {
            second_point = rand() % gens_amount;
        } while (abs(first_point - second_point) < 2);
        if (first_point > second_point) {
            int buf = first_point;
            first_point = second_point;
            second_point = buf;
        }

        Individual new_individual_1, new_individual_2;
        Chromosome new_chromosome_1, new_chromosome_2;
        new_chromosome_1.gens.resize(gens_amount);
        new_chromosome_2.gens.resize(gens_amount);
        for (int j = first_point; j < second_point; j++) {
            new_chromosome_1.gens[j] = current_generation[i].chromosomes[0].gens[j];
            new_chromosome_2.gens[j] = current_generation[i+1].chromosomes[0].gens[j];
        }

        doCrossingOverFillNonInherited(&new_chromosome_1, i+1, first_point, second_point);
        doCrossingOverFillNonInherited(&new_chromosome_2, i, first_point, second_point);

        if (new_chromosome_1 == new_chromosome_2) {
            doMutationInChromosome(&new_chromosome_1);
            doMutationInChromosome(&new_chromosome_2);
        }

        new_individual_1.chromosomes.push_back(new_chromosome_1);
        new_individual_2.chromosomes.push_back(new_chromosome_2);
        new_individuals.push_back(new_individual_1);
        new_individuals.push_back(new_individual_2);
    }

    for (int i = 0; i < new_individuals.size(); i++) {
        countIndividualFitness(&new_individuals[i]);
        current_generation.push_back(new_individuals[i]);
    }
}
void GA_TS_problem::doMutation() {
    for (int i = 0; i < GENERATION_SIZE; i++) {
        doMutationInChromosome(&current_generation[i].chromosomes[0]);
    }
}

float GA_TS_problem::countTwoGensFitness(Gen g1, Gen g2) {
    return sqrtf(pow((g2.x - g1.x), 2) + pow((g2.y - g1.y), 2));
}
void GA_TS_problem::countIndividualFitness(Individual* ind) {
    ind->fitness_value = 0;
    for (int i = 0; i < ind->chromosomes.size(); i++) {
        int j = 0;
        for (j = 0; j < ind->chromosomes[i].gens.size() - 1; j++) {
            ind->fitness_value += fitness_arr[ind->chromosomes[i].gens[j].id][ind->chromosomes[i].gens[j+1].id];
        }
        ind->fitness_value += fitness_arr[ind->chromosomes[i].gens[j].id][ind->chromosomes[i].gens[0].id];
    }
}
void GA_TS_problem::countCurrentGenerationFitness() {
    for (int i = 0; i < GENERATION_SIZE; i++) {
        countIndividualFitness(&current_generation[i]);
    }
}
void GA_TS_problem::doCrossingOverFillNonInherited(Chromosome* new_chromosome, int current_generation_index, int first_point, int second_point) {
    int parent_gen_index = 0;
    Chromosome parent_chromosome = current_generation[current_generation_index].chromosomes[0];
    for (int i = 0; i < gens_amount; i++) {
        if (i == first_point) {
            i = second_point - 1;
            continue;
        }
        do {
            Gen parent_gen = parent_chromosome.gens[parent_gen_index];
            parent_gen_index++;

            if (find(new_chromosome->gens.begin(), new_chromosome->gens.end(), parent_gen) == new_chromosome->gens.end()) {
                new_chromosome->gens[i] = parent_gen;
                break;
            }
        } while (parent_gen_index < gens_amount);
    }
}
void GA_TS_problem::doMutationInChromosome(Chromosome* chr) {
    int first_point = rand() % gens_amount;
    int second_point;
    do {
        second_point = rand() % gens_amount;
    } while (first_point == second_point);

    Gen buf = chr->gens[first_point];
    chr->gens[first_point] = chr->gens[second_point];
    chr->gens[second_point] = buf;
}

void GA_TS_problem::printCurrentGeneration() {
    for (int i = 0; i < GENERATION_SIZE; i++) {
        for (int j = 0; j < current_generation[i].chromosomes.size(); j++) {
            Chromosome chr = current_generation[i].chromosomes[j];
            for (int k = 0; k < chr.gens.size(); k++) {
                cout << chr.gens[k].id << " " << chr.gens[k].x << " " << chr.gens[k].y << endl;
            }
            cout << endl;
        }
    }
}
void GA_TS_problem::printCurrentGenerationFitnessValue(int amount) {
    if (amount > GENERATION_SIZE)
        amount = GENERATION_SIZE;
    for (int i = 0; i < amount; i++) {
        cout << current_generation[i].fitness_value << endl;
    }
    cout << endl;
}

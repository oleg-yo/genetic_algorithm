#include <iostream>

#include "GA_TS_problem.h"

using namespace std;

int main()
{
    GA_TS_problem ga;

    vector <Gen> dataset;
    for (int i = 0; i < 20; i++) {
        int x = rand() % 999 + 1;
        int y = rand() % 999 + 1;
        dataset.push_back(Gen(i, x, y));
    }
    ga.setNewData(dataset);
    ga.run();

    return 0;
}

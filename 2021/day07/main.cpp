#include "../../common.hpp"

void parsePositions(vector<int>& positions, istringstream& line)
{
    unsigned n;

    while (line >> n) {
        positions.push_back(n);
        line.ignore(1); // skip the comma
    }
}

unsigned calculateFuelCost(vector<int>& positions, int position)
{
    unsigned fuel = 0;

    for (auto& i : positions)
        fuel += abs(i - position);

    return fuel;
}

int main()
{
    vector<int> positions;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        parsePositions(positions, line);
    });

    int posMin = numeric_limits<int>::max();
    int posMax = numeric_limits<int>::min();

    for (auto& pos : positions) {
        if (pos < posMin)
            posMin = pos;
        if (pos > posMax)
            posMax = pos;
    }

    unsigned minimalFuel = numeric_limits<unsigned>::max();

    for (int pos = posMin; pos <= posMax; pos++) {
        unsigned fuel = calculateFuelCost(positions, pos);

        if (fuel < minimalFuel)
            minimalFuel = fuel;
    }

    cout << minimalFuel << '\n';

    return 0;
}

#include "../../common.hpp"

struct Fish
{
    static constexpr unsigned REPRODUCE_AFTER_BIRTH = 8;
    static constexpr unsigned REPRODUCE_NORMAL      = 6;

    unsigned daysToRepoduce;

    Fish(unsigned daysToRepoduce)
        : daysToRepoduce(daysToRepoduce)
    {}

    Fish()
        : daysToRepoduce(REPRODUCE_AFTER_BIRTH)
    {}

    bool update()
    {
        if (daysToRepoduce == 0) {
            daysToRepoduce = REPRODUCE_NORMAL;
            return true;
        } else {
            daysToRepoduce--;
            return false;
        }
    }
};

void parseInitialFishes(vector<Fish>& fishes, istringstream& line)
{
    unsigned n;

    while (line >> n) {
        fishes.push_back(Fish(n));
        line.ignore(1); // skip the comma
    }
}

int main()
{
    constexpr unsigned DAYS = 80;

    vector<Fish> fishes;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        parseInitialFishes(fishes, line);
    });

    for (unsigned i = 0; i < DAYS; i++) {
        unsigned newBorns = 0;

        for (auto& fish : fishes) {
            if (fish.update())
                newBorns++;
        }

        fishes.insert(fishes.end(), newBorns, Fish());
    }

    cout << fishes.size() << '\n';

    return 0;
}

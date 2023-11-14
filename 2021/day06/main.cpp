#include "../../common.hpp"

struct FishInventory
{
    static constexpr unsigned REPRODUCE_AFTER_BIRTH = 8;
    static constexpr unsigned REPRODUCE_NORMAL      = 6;

    using fish_count_t = unsigned long long;

    vector<fish_count_t> inventory;

    FishInventory()
        : inventory(REPRODUCE_AFTER_BIRTH + 1, 0)
    {}

    void addInitial(unsigned daysToRepoduce)
    { //
        inventory[daysToRepoduce]++;
    }

    void update()
    {
        fish_count_t numOfParents = inventory[0];

        for (int i = 0; i < inventory.size() - 1; i++)
            inventory[i] = inventory[i + 1];

        inventory[REPRODUCE_NORMAL] += numOfParents;
        inventory[REPRODUCE_AFTER_BIRTH] = numOfParents;
    }

    fish_count_t getTotalNumber()
    {
        fish_count_t sum = 0;

        for (auto& n : inventory)
            sum += n;

        return sum;
    }
};

void parseInitialFishes(FishInventory& inventory, istringstream& line)
{
    unsigned n;

    while (line >> n) {
        inventory.addInitial(n);
        line.ignore(1); // skip the comma
    }
}

int main()
{
    constexpr unsigned DAYS = 256;

    FishInventory inventory;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        parseInitialFishes(inventory, line);
    });

    for (unsigned i = 0; i < DAYS; i++)
        inventory.update();

    cout << inventory.getTotalNumber() << '\n';

    return 0;
}

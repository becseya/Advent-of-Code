#include "../../common.hpp"

using quantity_t  = unsigned long long;
using rules_t     = map<uint16_t, char>;
using histogram_t = map<char, quantity_t>;
using inventory_t = map<uint16_t, quantity_t>;

uint16_t getKey(const char* ptr)
{
    return (ptr[0] << 8) | ((uint8_t)ptr[1]);
}

void reverseKey(uint16_t key, char* ptr)
{
    ptr[0] = key >> 8;
    ptr[1] = (uint8_t)key;
}

void parseLine(istringstream& line, rules_t& rules)
{
    string pattern = getVariable<string>(line);
    line.ignore(4); // length of ' -> '
    char insert = getVariable<char>(line);

    rules[getKey(pattern.c_str())] = insert;
}

struct ChainManipulator
{
    char        first;
    char        last;
    inventory_t inventory;

    ChainManipulator(string initialChain)
    {
        const char* str = initialChain.c_str();
        size_t      len = initialChain.length();

        first = initialChain[0];
        for (size_t i = 0; i < len - 1; i++) {
            inventory[getKey(str)]++;
            str++;
        }
        last = *str;
    }

    void iterate(rules_t& rules)
    {
        inventory_t newInventory;

        for (auto& keyVal : inventory) {
            auto keyOrig = keyVal.first;
            auto amount  = keyVal.second;
            auto it      = rules.find(keyOrig);

            if (it != rules.end()) {
                char cOrig[2];
                char c = it->second;

                // calculate new keys
                reverseKey(keyOrig, cOrig);
                char cNew1[2] = { cOrig[0], c };
                char cNew2[2] = { c, cOrig[1] };

                newInventory[getKey(cNew1)] += amount;
                newInventory[getKey(cNew2)] += amount;
            } else
                newInventory[keyOrig] += amount;
        }

        inventory = newInventory;
    }

    histogram_t getHistogram()
    {
        histogram_t histogram;
        char        chars[2];

        histogram[first]++;
        histogram[last]++;

        for (auto& keyVal : inventory) {
            reverseKey(keyVal.first, chars);
            histogram[chars[0]] += keyVal.second;
            histogram[chars[1]] += keyVal.second;
        }

        for (auto& keyVal : histogram)
            keyVal.second /= 2;

        return histogram;
    }
};

void printHistogram(histogram_t histogram, bool debug = false)
{
    quantity_t total = 0;
    auto       it    = histogram.begin();

    pair<char, quantity_t> min = *it;
    pair<char, quantity_t> max = *it;

    for (auto& keyVal : histogram) {
        if (keyVal.second < min.second)
            min = keyVal;
        if (keyVal.second > max.second)
            max = keyVal;

        if (debug) {
            total += keyVal.second;
            cout << keyVal.first << ':' << keyVal.second << "    ";
        }
    }

    if (debug)
        cout << '(' << total << ")\n";
    else
        cout << max.second - min.second << '\n';
}

int main()
{
    static constexpr int ITERATIONS = 40;

    rules_t rules;
    string  chain;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        if (lineIdx == 0)
            chain = line.str();
        else if (lineIdx > 1)
            parseLine(line, rules);
    });

    ChainManipulator m(chain);

    for (int i = 0; i < ITERATIONS; i++)
        m.iterate(rules);

    printHistogram(m.getHistogram());

    return 0;
}

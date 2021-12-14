#include "../../common.hpp"

using quantity_t  = unsigned long;
using rules_t     = map<uint16_t, char>;
using histogram_t = map<char, quantity_t>;

uint16_t getKey(const char* ptr)
{
    return (ptr[0] << 8) | ((uint8_t)ptr[1]);
}

void parseLine(istringstream& line, rules_t& rules)
{
    string pattern = getVariable<string>(line);
    line.ignore(4); // length of ' -> '
    char insert = getVariable<char>(line);

    rules[getKey(pattern.c_str())] = insert;
}

string expandChain(rules_t& rules, const char* str, size_t size)
{
    string newChain;

    for (size_t i = 0; i < size - 1; i++) {

        newChain.push_back(*str);

        auto it = rules.find(getKey(str));
        if (it != rules.end())
            newChain.push_back(it->second);

        str++;
    }

    newChain.push_back(*str);

    return newChain;
}

histogram_t makeHistogram(string& chain)
{
    histogram_t histogram;

    for (auto& c : chain)
        histogram[c]++;

    return histogram;
}

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
    static constexpr int ITERATIONS = 10;

    rules_t rules;
    string  chain;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        if (lineIdx == 0)
            chain = line.str();
        else if (lineIdx > 1)
            parseLine(line, rules);
    });

    for (int i = 0; i < ITERATIONS; i++)
        chain = expandChain(rules, chain.c_str(), chain.size());

    printHistogram(makeHistogram(chain));

    return 0;
}

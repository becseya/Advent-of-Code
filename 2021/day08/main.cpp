#include "../../common.hpp"

struct Digit
{
    /*
     *  aaaa    ....    aaaa    aaaa    ....    aaaa    aaaa    aaaa    aaaa    aaaa
     * b    c  .    c  .    c  .    c  b    c  b    .  b    .  .    c  b    c  b    c
     * b    c  .    c  .    c  .    c  b    c  b    .  b    .  .    c  b    c  b    c
     *  ....    ....    dddd    dddd    dddd    dddd    dddd    ....    dddd    dddd
     * e    f  .    f  e    .  .    f  .    f  .    f  e    f  .    f  e    f  .    f
     * e    f  .    f  e    .  .    f  .    f  .    f  e    f  .    f  e    f  .    f
     *  gggg    ....    gggg    gggg    ....    gggg    gggg    ....    gggg    gggg
     *
     *   6       2       5        5      4       5       6       3        7       6
     */

    static constexpr unsigned SEGMENTS = 7;

    unsigned mask;

    Digit(unsigned mask = 0)
        : mask(mask)
    {}

    Digit(string str)
    {
        mask = 0;

        for (auto& c : str)
            mask |= (1 << (c - 'a'));
    }

    unsigned getActiveSegmentNum()
    {
        int activeNum = 0;

        for (int i = 0; i < SEGMENTS; i++) {
            if (mask & (1 << i))
                activeNum++;
        }

        return activeNum;
    }
};

struct Entry
{
    static constexpr auto SEPARATOR = "|";

    vector<Digit> samples;
    vector<Digit> digits;

    Entry(istringstream& line)
    {
        string         str;
        vector<Digit>* arrayPtr = &samples;

        while (line >> str) {
            if (str == SEPARATOR)
                arrayPtr = &digits;
            else
                arrayPtr->push_back(Digit(str));
        }
    }

    unsigned getNumberOfSignificantDigits()
    {
        unsigned sum = 0;

        for (auto& digit : digits) {
            switch (digit.getActiveSegmentNum()) {
                case 2:
                case 3:
                case 4:
                case 7: //
                    sum++;
            }
        }

        return sum;
    }
};

int main()
{
    vector<Entry> entries;
    unsigned      totalNumberOfSignificantDigits = 0;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        entries.push_back(Entry(line));
    });

    for (auto& e : entries)
        totalNumberOfSignificantDigits += e.getNumberOfSignificantDigits();

    cout << totalNumberOfSignificantDigits << '\n';

    return 0;
}

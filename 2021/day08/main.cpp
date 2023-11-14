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

    Digit sub(const Digit& other) const
    { //
        return Digit(mask & ~other.mask);
    }

    bool contains(const Digit& other) const
    { //
        return ((mask & other.mask) == other.mask);
    }

    bool operator!=(const Digit& other) const
    { //
        return (mask != other.mask);
    }

    bool operator==(const Digit& other) const
    { //
        return (mask == other.mask);
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

    unsigned getValue()
    {
        map<unsigned, Digit> knownDigits;

        for (auto& digit : samples) {
            switch (digit.getActiveSegmentNum()) {
                case 2: knownDigits[1] = digit; break;
                case 3: knownDigits[7] = digit; break;
                case 4: knownDigits[4] = digit; break;
                case 7: knownDigits[8] = digit; break;
            }
        }

        // Segments with 6 active digits: 0, 6, 9

        foreachDigitWithActiveSegmentNum(6, [&](Digit& digit) -> void {
            if (digit.contains(knownDigits[4]))
                knownDigits[9] = digit;
        });

        foreachDigitWithActiveSegmentNum(6, [&](Digit& digit) -> void {
            if (digit.contains(knownDigits[1]) && digit != knownDigits[9])
                knownDigits[0] = digit;
        });

        foreachDigitWithActiveSegmentNum(6, [&](Digit& digit) -> void {
            if (digit != knownDigits[0] && digit != knownDigits[9])
                knownDigits[6] = digit;
        });

        // Segments with 5 active digits: 2, 3, 5

        Digit seg_bd = knownDigits[4].sub(knownDigits[1]);
        foreachDigitWithActiveSegmentNum(5, [&](Digit& digit) -> void {
            if (digit.contains(seg_bd))
                knownDigits[5] = digit;
        });

        foreachDigitWithActiveSegmentNum(5, [&](Digit& digit) -> void {
            if (digit.contains(knownDigits[1]))
                knownDigits[3] = digit;
        });

        foreachDigitWithActiveSegmentNum(5, [&](Digit& digit) -> void {
            if (digit != knownDigits[5] && digit != knownDigits[3])
                knownDigits[2] = digit;
        });

        // Calculate value of digits
        unsigned value = 0;
        for (int i = 0; i < digits.size(); i++) {
            value *= 10;
            value += reverseSearch(knownDigits, digits[i]);
        }

        return value;
    }

    void foreachDigitWithActiveSegmentNum(unsigned activeSegmentNum, function<void(Digit&)> cb)
    {
        for (Digit& digit : samples) {
            if (digit.getActiveSegmentNum() == activeSegmentNum)
                cb(digit);
        }
    }

    static int reverseSearch(map<unsigned, Digit>& digits, Digit digit)
    {
        for (const auto& keyVal : digits) {
            if (keyVal.second == digit)
                return keyVal.first;
        }

        throw runtime_error("Unknown digit");
    }
};

int main()
{
    vector<Entry> entries;
    unsigned      sumOfValues = 0;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        entries.push_back(Entry(line));
    });

    for (auto& e : entries)
        sumOfValues += e.getValue();

    cout << sumOfValues << '\n';

    return 0;
}

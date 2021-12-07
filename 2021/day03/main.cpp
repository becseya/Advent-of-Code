#include "../../common.hpp"

enum class DominantBit
{
    One,
    Zero,
    Ambiguous,
};

DominantBit getDominantBitAtPosition(list<string>& numbers, unsigned pos)
{
    int sum = 0;

    for (auto const& numberString : numbers) {
        if (numberString[pos] == '1')
            sum++;
        else if (numberString[pos] == '0')
            sum--;
        else
            throw runtime_error("String is not binary");
    }

    if (sum > 0)
        return DominantBit::One;
    else if (sum < 0)
        return DominantBit::Zero;
    else
        return DominantBit::Ambiguous;
}

void removeFromListIfCharacterDiffers(list<string>& strings, unsigned pos, char c)
{
    auto it = strings.begin();

    while (it != strings.end()) {
        if (it->at(pos) != c)
            it = strings.erase(it);
        else
            it++;
    }
}

string reduceListToSingle(list<string> numbers, bool normalPolarity)
{
    unsigned bits = numbers.begin()->size();

    char cOne  = normalPolarity ? '1' : '0';
    char cZero = normalPolarity ? '0' : '1';

    for (int pos = 0; pos < bits; pos++) {
        auto dominant = getDominantBitAtPosition(numbers, pos);

        if (dominant == DominantBit::One)
            removeFromListIfCharacterDiffers(numbers, pos, cOne);
        else if (dominant == DominantBit::Zero)
            removeFromListIfCharacterDiffers(numbers, pos, cZero);
        else if (dominant == DominantBit::Ambiguous)
            removeFromListIfCharacterDiffers(numbers, pos, cOne);

        if (numbers.size() == 1)
            return numbers.back();
    }

    throw runtime_error("Not found");
}

int main()
{
    list<string> numbers;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //

        numbers.push_back(line.str());
    });

    string strOxygen = reduceListToSingle(numbers, true);
    string strCO2    = reduceListToSingle(numbers, false);

    auto multiplied = stoull(strOxygen, 0, 2) * stoull(strCO2, 0, 2);

    cout << multiplied << "\n";
}

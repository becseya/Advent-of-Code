#include "../../common.hpp"

int main()
{
    vector<int> sums;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //

        string binaryStr = line.str();

        for (int i = 0; i < binaryStr.length(); i++) {
            // fill vector at first line
            if ((lineIdx == 0) && (i >= sums.size()))
                sums.push_back(0);

            // keep track of ones and zeros
            if (binaryStr[i] == '1')
                sums[i]++;
            else if (binaryStr[i] == '0')
                sums[i]--;
            else
                throw runtime_error("String is not binary");
        }
    });

    unsigned bits  = sums.size();
    unsigned gamma = 0;

    for (int i = 0; i < bits; i++) {
        if (sums[i] > 0)
            gamma |= (1 << (bits - 1 - i));
        else if (sums[i] == 0)
            throw runtime_error("Ambiguous");
    }

    unsigned epsilon = gamma ^ ((1 << sums.size()) - 1);

    cout << gamma * epsilon << "\n";
}

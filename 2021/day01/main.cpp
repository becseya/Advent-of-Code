#include "../../common.hpp"

int main()
{
    int prev;
    int count = 0;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //

        int val = getVariable<int>(line);

        if (lineIdx > 0) {
            if (val > prev)
                count++;
        }

        prev = val;
    });

    cout << "Increased " << count << " times\n";
}

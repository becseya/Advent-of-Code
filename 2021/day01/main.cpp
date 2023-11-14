#include "../../common.hpp"

int main()
{
    constexpr int WINDOW_SIZE = 3;

    int         count = 0;
    vector<int> window(WINDOW_SIZE, 0);

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //

        int val = getVariable<int>(line);

        if (lineIdx >= WINDOW_SIZE) {
            int oldSum = 0;
            for (auto& element : window)
                oldSum += element;

            int newSum = oldSum - window[lineIdx % WINDOW_SIZE] + val;

            if (newSum > oldSum)
                count++;
        }

        window[lineIdx % WINDOW_SIZE] = val;
    });

    cout << "Increased " << count << " times\n";
}

#include "../../common.hpp"

int main()
{
    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        cout << line.str() << '\n';
    });
}

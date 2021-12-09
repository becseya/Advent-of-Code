#include "../../common.hpp"

vector<unsigned> parseLine(istringstream& line)
{
    char             c;
    vector<unsigned> lineValues;

    while (line >> c)
        lineValues.push_back(c - '0');

    return lineValues;
}

using map_t = vector<vector<unsigned>>;

using neighbourVisitor = function<void(int r, int c)>;

void foreachNeighbour(int rows, int cols, int r, int c, neighbourVisitor cb)
{
    if (r > 0)
        cb(r - 1, c);

    if (c > 0)
        cb(r, c - 1);

    if (r < rows - 1)
        cb(r + 1, c);

    if (c < cols - 1)
        cb(r, c + 1);
}

bool isLocalMinimum(map_t& map, int rows, int cols, int r0, int c0)
{
    int  valueOrigin = map[r0][c0];
    bool isMinimal   = true;

    foreachNeighbour(rows, cols, r0, c0, [&](int r, int c) -> void {
        if (map[r][c] <= valueOrigin)
            isMinimal = false;
    });

    return isMinimal;
}

int main()
{
    map_t map;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        map.push_back(parseLine(line));
    });

    unsigned totalRiskLevel = 0;

    int rows = map.size();
    int cols = map[0].size();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (isLocalMinimum(map, rows, cols, r, c))
                totalRiskLevel += (map[r][c] + 1);
        }
    }

    cout << totalRiskLevel << '\n';

    return 0;
}

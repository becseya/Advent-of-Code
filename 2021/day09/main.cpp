#include "../../common.hpp"

struct Entry
{
    int  value;
    bool visited;
};

vector<Entry> parseLine(istringstream& line)
{
    char          c;
    vector<Entry> lineValues;

    while (line >> c)
        lineValues.push_back({ c - '0', false });

    return lineValues;
}

using map_t = vector<vector<Entry>>;

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
    unsigned valueOrigin = map[r0][c0].value;
    bool     isMinimal   = true;

    foreachNeighbour(rows, cols, r0, c0, [&](int r, int c) -> void {
        if (map[r][c].value <= valueOrigin)
            isMinimal = false;
    });

    return isMinimal;
}

bool isNeighbourHigher(Entry& neighbour, Entry& origin)
{
    return ((neighbour.value > origin.value) && (neighbour.value != 9));
}

unsigned getNumberOfHigherNeighbour(map_t& map, int rows, int cols, int r0, int c0)
{
    Entry&   origin  = map[r0][c0];
    unsigned subSize = 0;

    origin.visited = true;

    foreachNeighbour(rows, cols, r0, c0, [&](int r, int c) -> void { //
        Entry& neighbour = map[r][c];

        if (!neighbour.visited && isNeighbourHigher(neighbour, origin))
            subSize += 1 + getNumberOfHigherNeighbour(map, rows, cols, r, c);
    });

    return subSize;
}

unsigned getPoolSize(map_t& map, int rows, int cols, int r0, int c0)
{
    return (getNumberOfHigherNeighbour(map, rows, cols, r0, c0) + 1); // add origin
}

int main()
{
    map_t map;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        map.push_back(parseLine(line));
    });

    int rows = map.size();
    int cols = map[0].size();

    static constexpr unsigned NUMBER_OF_TOP_POOLS = 3;
    list<unsigned>            topPools;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (isLocalMinimum(map, rows, cols, r, c)) {
                topPools.push_back(getPoolSize(map, rows, cols, r, c));
                topPools.sort();
                if (topPools.size() > NUMBER_OF_TOP_POOLS)
                    topPools.pop_front();
            }
        }
    }

    unsigned topMultiplied = 1;
    for (auto& i : topPools)
        topMultiplied *= i;

    cout << topMultiplied << '\n';

    return 0;
}

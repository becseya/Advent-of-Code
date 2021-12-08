#include "../../common.hpp"

struct PointPair
{
    int x0;
    int y0;
    int x1;
    int y1;
};

PointPair parseLine(istringstream& line)
{
    PointPair entry;

    line >> entry.x0;
    line.ignore(1);
    line >> entry.y0;
    line.ignore(4);
    line >> entry.x1;
    line.ignore(1);
    line >> entry.y1;

    return entry;
}

using map_t = vector<vector<int>>;

void swapIfReversed(int& val1, int& val2)
{
    if (val2 < val1) {
        int temp = val1;
        val1     = val2;
        val2     = temp;
    }
}

void drawLine(map_t& map, PointPair entry)
{
    if (entry.y0 == entry.y1) {
        swapIfReversed(entry.x0, entry.x1);
        for (int x = entry.x0; x <= entry.x1; x++)
            map[x][entry.y0]++;
    } else if (entry.x0 == entry.x1) {
        swapIfReversed(entry.y0, entry.y1);
        for (int y = entry.y0; y <= entry.y1; y++)
            map[entry.x0][y]++;
    }
}

int main()
{
    constexpr size_t MAP_SIZE      = 1000;
    constexpr size_t OVERLAP_LIMIT = 1;

    map_t map(MAP_SIZE, vector<int>(MAP_SIZE, 0));

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        drawLine(map, parseLine(line));
    });

    unsigned count = 0;

    for (auto& row : map) {
        for (auto& i : row) {
            if (i > OVERLAP_LIMIT)
                count++;
        }
    }

    cout << count << '\n';

    return 0;
}

#include "../../common.hpp"

struct Entry
{
    static constexpr int MAX_VALUE = 9;

    int  value;
    bool flashed;

    bool stimulate()
    {
        if (!flashed && ++value > MAX_VALUE) {
            value   = 0;
            flashed = true;
            return true;
        }

        return false;
    }
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

void foreachNeighbour(int rows, int cols, int r0, int c0, neighbourVisitor cb)
{
    int rMin = (r0 > 0) ? (r0 - 1) : 0;
    int cMin = (c0 > 0) ? (c0 - 1) : 0;
    int rMax = (r0 < rows - 1) ? (r0 + 1) : r0;
    int cMax = (c0 < cols - 1) ? (c0 + 1) : c0;

    for (int r = rMin; r <= rMax; r++) {
        for (int c = cMin; c <= cMax; c++) {
            if (r == r0 && c == c0)
                continue;

            cb(r, c);
        }
    }
}

unsigned iterateCell(map_t& map, int rows, int cols, int r0, int c0)
{
    unsigned flashed = 0;

    if (map[r0][c0].stimulate()) {
        flashed++;

        foreachNeighbour(rows, cols, r0, c0, [&](int r, int c) -> void { //
            flashed += iterateCell(map, rows, cols, r, c);
        });
    }

    return flashed;
}

int main()
{
    unsigned iteration = 0;
    map_t    map;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        map.push_back(parseLine(line));
    });

    while (++iteration) {

        int rows = map.size();
        int cols = map[0].size();

        unsigned flashes = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                flashes += iterateCell(map, rows, cols, r, c);
            }
        }

        if (flashes == rows * cols)
            break;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                map[r][c].flashed = false;
            }
        }
    }

    cout << iteration << "\n";

    return 0;
}

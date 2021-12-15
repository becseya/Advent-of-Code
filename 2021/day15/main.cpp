#include "../../common.hpp"

static constexpr int UNKNOWN_RISK = -1;

struct Entry
{
    int risk;
    int totalRisk;
};

vector<Entry> parseLine(istringstream& line)
{
    char          c;
    vector<Entry> lineValues;

    while (line >> c)
        lineValues.push_back({ c - '0', UNKNOWN_RISK });

    return lineValues;
}

using grid_t = vector<vector<Entry>>;

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

int getMinimalRisk(grid_t& map, int rows, int cols, int r0, int c0)
{
    int risk = UNKNOWN_RISK;

    foreachNeighbour(rows, cols, r0, c0, [&](int r, int c) -> void {
        auto& p = map[r][c];

        if (p.totalRisk < 0)
            return;

        int newRisk = (p.totalRisk + p.risk);

        if ((risk == UNKNOWN_RISK) || newRisk < risk)
            risk = newRisk;
    });

    return risk;
}

void iterate(grid_t& map, int rows, int cols)
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int newRisk = getMinimalRisk(map, rows, cols, r, c);

            if (newRisk == UNKNOWN_RISK)
                continue;

            auto& p = map[r][c];
            if ((p.totalRisk == UNKNOWN_RISK) || (newRisk < p.totalRisk))
                p.totalRisk = newRisk;
        }
    }
}

void printMap(grid_t& map, int rows, int cols)
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            auto& p = map[r][c];

            if (p.totalRisk > UNKNOWN_RISK)
                cout << p.risk;
            else
                cout << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

int main()
{
    grid_t map;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        map.push_back(parseLine(line));
    });

    int rows = map.size();
    int cols = map[0].size();

    // This should be the maximal length (number of cells) of ideal path
    // Theoretically it can be much larger, due to possible zig-zags
    // Minimum should be 'rows + cols', the distance of diagonal points on a grid
    // But the iteration in 'printMap' favours finding minimal paths quick
    // (in fact a singe iteration finds the good solution)
    // For now, 50 is a safety estimation
    int numberOfIterations = 50;

    map[0][0].totalRisk = 0;

    for (int i = 0; i < numberOfIterations; i++) {
        iterate(map, rows, cols);
    }

    cout << map[rows - 1][cols - 1].totalRisk << '\n';

    return 0;
}

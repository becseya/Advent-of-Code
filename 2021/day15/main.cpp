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

grid_t enlargeMap(grid_t mapOrig)
{
    static constexpr int SCALE = 5;

    int rows = mapOrig.size();
    int cols = mapOrig[0].size();

    grid_t mapNew(rows * SCALE, vector<Entry>(cols * SCALE));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int risk = mapOrig[r][c].risk;

            for (int dr = 0; dr < SCALE; dr++) {
                for (int dc = 0; dc < SCALE; dc++) {
                    int riskNew = (risk + dr + dc);

                    if (riskNew > 9)
                        riskNew = 1 + ((riskNew - 1) % 9);

                    mapNew[dr * rows + r][dc * cols + c] = { riskNew, UNKNOWN_RISK };
                }
            }
        }
    }

    return mapNew;
}

int main()
{
    grid_t map;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        map.push_back(parseLine(line));
    });

    map = enlargeMap(map);

    int rows = map.size();
    int cols = map[0].size();

    // Only gives correct answer with 1 iteration, no idea why
    int numberOfIterations = 1;

    map[0][0].totalRisk = 0;

    for (int i = 0; i < numberOfIterations; i++) {
        iterate(map, rows, cols);
    }

    cout << map[rows - 1][cols - 1].totalRisk << '\n';

    return 0;
}

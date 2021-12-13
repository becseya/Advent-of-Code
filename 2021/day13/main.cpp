#include "../../common.hpp"

struct Point
{
    int x, y;

    Point(istringstream& line)
    {
        x = getVariable<int>(line);
        line.ignore(1);
        y = getVariable<int>(line);
    }
};

struct FoldingParam
{
    char axis;
    int  foldLine;

    FoldingParam(istringstream& line)
    {
        line.ignore(string("fold along ").length());
        axis = getVariable<char>(line);
        line.ignore(1);
        foldLine = getVariable<int>(line);
    }
};

struct Paper
{
    static constexpr size_t MAX_GRID_SIZE = 2000;

    vector<vector<int>> grid;
    int                 xSize;
    int                 ySize;

    Paper()
        : grid(MAX_GRID_SIZE, vector<int>(MAX_GRID_SIZE, 0))
    {}

    void drawPoint(Point& p)
    { //
        grid[p.x][p.y]++;
    }

    void fold(FoldingParam param)
    {
        // folds grid at axis = foldLine;

        if (param.axis == 'x') {
            xSize = param.foldLine;

            for (int y = 0; y < ySize; y++)
                for (int dx = 1; dx <= xSize; dx++)
                    grid[param.foldLine - dx][y] += grid[param.foldLine + dx][y];
        } else if (param.axis == 'y') {
            ySize = param.foldLine;

            for (int x = 0; x < xSize; x++)
                for (int dy = 1; dy <= ySize; dy++)
                    grid[x][param.foldLine - dy] += grid[x][param.foldLine + dy];
        }
    }

    void print()
    {
        for (int y = 0; y < ySize; y++) {
            for (int x = 0; x < xSize; x++)
                cout << (grid[x][y] > 0 ? '#' : '.');

            cout << '\n';
        }
        cout << '\n';
    }

    size_t getNumberOfPainted()
    {
        size_t painted = 0;

        for (int y = 0; y < ySize; y++) {
            for (int x = 0; x < xSize; x++) {
                if (grid[x][y] > 0)
                    painted++;
            }
        }

        return painted;
    }
};

int main()
{

    vector<Point> points;
    Paper         grid;

    bool parsingPoints = true;
    int  xMax          = 0;
    int  yMax          = 0;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        if (parsingPoints) {
            if (line.str() == "") {
                grid.xSize    = xMax + 1;
                grid.ySize    = yMax + 1;
                parsingPoints = false;
            } else {
                Point p = Point(line);

                if (p.x > xMax)
                    xMax = p.x;
                if (p.y > yMax)
                    yMax = p.y;

                grid.drawPoint(p);
            }
        } else {
            grid.fold(FoldingParam(line));
            cout << grid.getNumberOfPainted() << '\n';
            exit(0);
        }
    });

    return 0;
}

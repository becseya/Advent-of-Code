#include "../../common.hpp"

void parseWinnerNumbers(vector<unsigned>& numbers, istringstream& line)
{
    unsigned n;
    char     dummy;

    while (line >> n) {
        numbers.push_back(n);
        line >> dummy; // skip the comma
    }
}

struct Board
{
    struct Entry
    {
        unsigned val;
        bool     matched;
    };

    vector<vector<Entry>> entryMatrix;

    void appendLine(istringstream& line)
    {
        unsigned      n;
        vector<Entry> row;

        while (line >> n) {
            row.push_back({ n, false });
        }

        entryMatrix.push_back(row);
    }

    void markMatching(unsigned n)
    {
        for (auto& v : entryMatrix) {
            for (auto& e : v) {
                if (e.val == n)
                    e.matched = true;
            }
        }
    }

    bool isWinning()
    {
        // find a winning row
        for (auto& row : entryMatrix) {
            bool winning = true;

            for (auto& entry : row) {
                if (!entry.matched)
                    winning = false;
            }

            if (winning)
                return true;
        }

        // find a winning column
        unsigned columns = entryMatrix[0].size();
        unsigned rows    = entryMatrix.size();

        for (unsigned col = 0; col < columns; col++) {
            bool winning = true;

            for (unsigned row = 0; row < rows; row++) {
                if (!entryMatrix[row][col].matched)
                    winning = false;
            }

            if (winning)
                return true;
        }

        return false;
    }

    unsigned getSumOfUnmatching()
    {
        unsigned sum = 0;

        for (auto& row : entryMatrix) {
            for (auto& entry : row) {
                if (!entry.matched)
                    sum += entry.val;
            }
        }

        return sum;
    }
};

int main()
{
    vector<unsigned> winnerNumbers;
    vector<Board>    boards;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //

        if (lineIdx == 0) {
            parseWinnerNumbers(winnerNumbers, line);
        } else {
            if (line.str() == "") {
                Board newBoard;
                boards.push_back(newBoard);
            } else
                boards.back().appendLine(line);
        }
    });

    for (auto& n : winnerNumbers) {
        for (auto& board : boards) {
            board.markMatching(n);

            if (board.isWinning()) {
                cout << board.getSumOfUnmatching() * n << '\n';
                return 0;
            }
        }
    }

    cout << "No winner \n";
    return 1;
}

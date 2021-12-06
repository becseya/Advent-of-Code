#include "../../common.hpp"

enum class Direction
{
    Up,
    Down,
    Forward
};

std::tuple<Direction, unsigned> parseLine(istringstream& line)
{
    string   directionStr = getVariable<string>(line);
    unsigned displacement = getVariable<unsigned>(line);

    Direction dir;

    if (directionStr == "forward")
        dir = Direction::Forward;
    else if (directionStr == "up")
        dir = Direction::Up;
    else if (directionStr == "down")
        dir = Direction::Down;
    else
        throw runtime_error("Unknown direction");

    return { dir, displacement };
}

int main()
{
    int depth              = 0;
    int horizontalPosition = 0;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        auto [direction, displacement] = parseLine(line);

        if (direction == Direction::Down)
            depth += displacement;
        else if (direction == Direction::Up)
            depth -= displacement;
        else if (direction == Direction::Forward)
            horizontalPosition += displacement;
    });

    cout << "dept: " << depth << ", horiz: " << horizontalPosition << " -> " << (depth * horizontalPosition) << "\n";
}

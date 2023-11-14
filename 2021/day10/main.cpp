#include "../../common.hpp"

enum class ChunkType
{
    Round,
    Square,
    Curly,
    Angle,
};

struct Chunk
{
    ChunkType type;
    bool      isOpening;

    Chunk(char c)
    {
        switch (c) {
            case '(':
            case ')':
                type      = ChunkType::Round;
                isOpening = (c == '(');
                break;
            case '[':
            case ']':
                type      = ChunkType::Square;
                isOpening = (c == '[');
                break;
            case '{':
            case '}':
                type      = ChunkType::Curly;
                isOpening = (c == '{');
                break;
            case '<':
            case '>':
                type      = ChunkType::Angle;
                isOpening = (c == '<');
                break;
            default: //
                throw runtime_error("Unknown chunk type");
        }
    }

    unsigned getAutoCorrectPoint()
    {
        switch (type) {
            case ChunkType::Round: return 1;
            case ChunkType::Square: return 2;
            case ChunkType::Curly: return 3;
            case ChunkType::Angle: return 4;
        }

        return 0;
    }

    int getErrorPoint()
    {
        switch (type) {
            case ChunkType::Round: return 3;
            case ChunkType::Square: return 57;
            case ChunkType::Curly: return 1197;
            case ChunkType::Angle: return 25137;
        }

        return 0;
    }
};

unsigned long calculateAutocorrectPoint(list<Chunk>& stack)
{
    unsigned long points = 0;

    for (list<Chunk>::reverse_iterator it = stack.rbegin(); it != stack.rend(); it++) {
        points = points * 5 + it->getAutoCorrectPoint();
    }

    return points;
}

int parseChunks(string line, list<unsigned long>& autocorrectPoints)
{
    list<Chunk> stack;

    for (char& c : line) {
        Chunk newChunk(c);

        if (newChunk.isOpening)
            stack.push_back(newChunk);
        else {
            Chunk oldChunk = stack.back();

            if (oldChunk.type != newChunk.type)
                return newChunk.getErrorPoint();
            else
                stack.pop_back();
        }
    }

    autocorrectPoints.push_back(calculateAutocorrectPoint(stack));

    return 0;
}

int main()
{
    int totalErrorPoints = 0;

    list<unsigned long> autocorrectPoints;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        totalErrorPoints += parseChunks(line.str(), autocorrectPoints);
    });

    cout << totalErrorPoints << '\n';

    // find middle of autocorrectPoints
    autocorrectPoints.sort();
    int idx = autocorrectPoints.size() / 2;

    cout << listAt(autocorrectPoints, idx) << '\n';

    return 0;
}

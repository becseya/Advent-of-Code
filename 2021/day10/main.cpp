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

int parseChunks(string line)
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

    return 0;
}

int main()
{
    int totalErrorPoints = 0;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        totalErrorPoints += parseChunks(line.str());
    });

    cout << totalErrorPoints << '\n';

    return 0;
}

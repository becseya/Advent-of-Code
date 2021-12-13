#include "../../common.hpp"

constexpr char NODE_START[] = "start";
constexpr char NODE_END[]   = "end";

using StringPair = pair<string, string>;
using edges_t    = vector<StringPair>;

StringPair parseLine(istringstream& line)
{
    string str1, str2;

    if (!getline(line, str1, '-'))
        throw runtime_error("Failed to parse");
    if (!getline(line, str2, '-'))
        throw runtime_error("Failed to parse");

    return { str1, str2 };
}

bool isSmallCave(string& item)
{
    return ('a' <= item[0]) && (item[0] <= 'z');
}

string getPathString(vector<string> visited)
{
    string path = "";

    for (auto& i : visited)
        path += i + ",";

    return path;
}

void appendIfProperSmallCage(vector<string>& items, string item)
{
    if (!isSmallCave(item) || (item == NODE_START) || (item == NODE_END))
        return;

    if (occurs(items, item) == 0)
        items.push_back(item);
}

vector<string> getSmallCaves(edges_t& edges)
{
    vector<string> smallCaves;

    for (auto& pair : edges) {
        appendIfProperSmallCage(smallCaves, pair.first);
        appendIfProperSmallCage(smallCaves, pair.second);
    }

    return smallCaves;
}

// --------------------------------------------------------------------------------------------------------------------

vector<string> getReachableNeighbors(edges_t& edges, vector<string>& visited, string& node, string twiceVisitable)
{
    vector<string> reachable;

    for (auto& pair : edges) {
        string candidate;

        if (pair.first == node)
            candidate = pair.second;
        else if (pair.second == node)
            candidate = pair.first;
        else
            continue;

        if (candidate == twiceVisitable) {
            if (occurs(visited, candidate) >= 2)
                continue;
        } else if (isSmallCave(candidate) && (occurs(visited, candidate) > 0))
            continue;

        reachable.push_back(candidate);
    }

    return reachable;
}

void findPaths(vector<string>& paths, edges_t& edges, string node, string twiceVisitable, vector<string> visited = {})
{
    unsigned numberOfPaths = 0;
    auto     neighbors     = getReachableNeighbors(edges, visited, node, twiceVisitable);

    visited.push_back(node);

    if (node == NODE_END) {
        string path = getPathString(visited);
        paths.push_back(path);
        return;
    }

    for (auto& neighbour : neighbors)
        findPaths(paths, edges, neighbour, twiceVisitable, visited);
}

int main()
{
    edges_t        edges;
    vector<string> paths;

    forEachLine([&](istringstream& line, int lineIdx) -> void {
        //
        edges.push_back(parseLine(line));
    });

    auto smallCages = getSmallCaves(edges);
    for (auto& smallCage : smallCages)
        findPaths(paths, edges, NODE_START, smallCage);

    // filter duplicates
    sort(paths.begin(), paths.end());
    size_t filteredSize = 0;

    if (paths[0] != paths[1])
        filteredSize++;

    for (int i = 1; i < paths.size(); i++) {
        if (paths[i] != paths[i - 1])
            filteredSize++;
    }

    cout << filteredSize << '\n';

    return 0;
}

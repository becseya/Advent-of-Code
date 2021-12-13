#pragma once

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
T getVariable(istringstream& stream)
{
    T val;

    if (!(stream >> val))
        throw runtime_error("Parsing error");

    return val;
}

void forEachLine(function<void(istringstream&, int)> cb)
{
    int    index = 0;
    string line;

    while (getline(cin, line)) {
        istringstream iss(line);
        cb(iss, index++);
    }
}

template<typename T>
T listAt(const list<T>& list, size_t idx)
{
    if (list.size() > idx) {
        auto it = list.begin();
        std::advance(it, idx);
        return *it;
    }

    throw runtime_error("List index out of limit");
}

template<typename T>
unsigned occurs(const vector<T>& items, const T& itemToMatch)
{
    unsigned occurrences = 0;

    for (auto& item : items) {
        if (item == itemToMatch)
            occurrences++;
    }

    return occurrences;
}

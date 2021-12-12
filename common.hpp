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



#include "input.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cctype>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <cmath>
#include <array>
#include <list>

long long SolveLine(std::vector<std::vector<long long>>& data)
{
    bool isNonZero = true;
    size_t rowIdx = 0;
    while (isNonZero)
    {
        isNonZero = false;
        if (data.size() >= (rowIdx + 1))
        {
            data.push_back(std::vector<long long>());
        }
        data[rowIdx+1].clear();
        
        for(size_t colIdx = 0; colIdx < (data[rowIdx].size()-1); colIdx++)
        {
            data[rowIdx+1].push_back(data[rowIdx][colIdx+1] - data[rowIdx][colIdx]);
            isNonZero |= data[rowIdx+1][colIdx] != 0;
        }
        rowIdx++;
    }
    
    data[rowIdx].push_back(0);
    for (; rowIdx > 0; rowIdx--)
    {
        data[rowIdx-1].push_back(*data[rowIdx].rbegin() + *data[rowIdx-1].rbegin());
    }
    
    return *data[0].rbegin();
}


void SolveGame(const std::string& input)
{
    std::istringstream stream(input);
    std::string line;

    size_t result = 0;
    
    std::vector<std::vector<long long>> data;
    data.push_back(std::vector<long long>());
    while (std::getline(stream, line))
    {
        data[0].clear();
        while (line.length() > 0)
        {
            size_t pos = 0;
            data[0].push_back(std::stoi(line, &pos));
            line.erase(0, pos+1);
        }

        result += SolveLine(data);
    }

    std::cout << std::endl << "The result is " << result << std::endl;
}

int main(){

    SolveGame(testInput1);
    SolveGame(gameInput);

    return 0;
}
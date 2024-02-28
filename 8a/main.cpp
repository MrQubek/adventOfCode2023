

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

int CodeString(const std::string& str)
{
    return str[0]*10000+str[1]*100+str[2];
}

void ReadInput(const std::string& input, std::list<bool>& turnLefts, std::unordered_map<int, std::pair<int,int>>& map)
{
    std::istringstream stream(input);
    std::string line;

    std::getline(stream, line);
    for (size_t i = 0; i < line.length(); i++)
    {
        turnLefts.push_back(line[i] == 'L');
    }
    
    std::getline(stream, line);
    while (std::getline(stream, line)) {
        map[CodeString(line.substr(0, 3))] = std::make_pair(CodeString(line.substr(7, 3)), CodeString(line.substr(12, 3)));
    }
}

void SolveGame(const std::string& input)
{
    std::list<bool> turnLefts;
    std::unordered_map<int, std::pair<int,int>> map;
    int currentPosition = CodeString(std::string("AAA"));
    ReadInput(input, turnLefts, map);

    size_t result = 0;
    const int END = CodeString(std::string("ZZZ"));

    auto turnLeftsIt = turnLefts.begin();
    while (currentPosition != END)
    {
        currentPosition = *turnLeftsIt ? map[currentPosition].first : map[currentPosition].second;
        ++turnLeftsIt;
        if(turnLeftsIt == turnLefts.end())
        {
            turnLeftsIt = turnLefts.begin();
        }
        ++result;
    }
    
    std::cout << std::endl << "The result is " << result << std::endl;
}

int main(){

    // SolveGame(testInput1);
    SolveGame(testInput2);
    SolveGame(gameInput);

    return 0;
}
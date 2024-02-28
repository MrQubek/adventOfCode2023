

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
#include <numeric>

uint32_t CodeString(const std::string& str)
{
    return (str[0]<<16)+(str[1]<<8)+str[2];
}

bool IsEndingNode(uint32_t node)
{
    return (node & 0xff) == 'Z';
}

std::unordered_map<uint32_t, std::pair<uint32_t,uint32_t>> ReadInput(const std::string& input, std::list<bool>& turnLefts, std::vector<uint32_t>& concurrentNodes)
{
    std::unordered_map<uint32_t, std::pair<uint32_t,uint32_t>> map;
    std::istringstream stream(input);
    std::string line;

    std::getline(stream, line);
    for (unsigned long long i = 0; i < line.length(); i++)
    {
        turnLefts.push_back(line[i] == 'L');
    }
    
    std::getline(stream, line);
    while (std::getline(stream, line)) {
        if(line[2] == 'A')
        {
            concurrentNodes.push_back(CodeString(line.substr(0, 3)));
        }
        map[CodeString(line.substr(0, 3))] = std::make_pair(CodeString(line.substr(7, 3)), CodeString(line.substr(12, 3)));
    }
    return map;
}

void SolveGame(const std::string& input)
{
    std::list<bool> turnLefts;
    std::vector<uint32_t> startingNodes;
    const std::unordered_map<uint32_t, std::pair<uint32_t,uint32_t>> map = ReadInput(input, turnLefts, startingNodes);

    std::vector<unsigned long long> cyclesLength(startingNodes.size());
    std::transform(startingNodes.begin(), startingNodes.end(), cyclesLength.begin(), [&](const uint32_t startingNode)
    {
        unsigned long long cycleLength = 0;
        uint32_t currNode = startingNode;
        auto turnLeftsIt = turnLefts.begin();
        while (!IsEndingNode(currNode))
        {
            currNode = *turnLeftsIt ? map.at(currNode).first : map.at(currNode).second;
            ++cycleLength;
            ++turnLeftsIt;
            if(turnLeftsIt == turnLefts.end())
            {
                turnLeftsIt = turnLefts.begin();
            } 
        }
        return cycleLength;
    });

    unsigned long long result = std::accumulate<std::vector<unsigned long long>::iterator ,unsigned long long>(cyclesLength.begin(), cyclesLength.end(), 1, std::lcm<unsigned long long, unsigned long long>);

    std::cout << std::endl << "The result is " << result << std::endl;
}

int main(){

    SolveGame(testInput1);
    SolveGame(gameInput);

    return 0;
}
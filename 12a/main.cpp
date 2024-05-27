

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
#include <assert.h>
#include <chrono>
#include <thread>
#include <deque>

enum GeyserStatus
{
    Ok,
    Damaged,
    Unknown
};

size_t CalculateSpecyficCombinations( const std::vector<GeyserStatus>& springs
                                    , std::deque<size_t>& groups
                                    , std::map<std::pair<size_t, size_t>, size_t>& combinationsMap
                                    , size_t startIndex);

size_t ConsumeGeyserGroup(            const std::vector<GeyserStatus>& springs
                                    , std::deque<size_t>& groups
                                    , std::map<std::pair<size_t, size_t>, size_t> combinationsMap
                                    , size_t startIndex
                                    , size_t groupSize)
{
    if (springs.size() - startIndex < groupSize)
    {
        return 0;
    }
    
    if  (groupSize == 0)
    {
        // end of input case
        if (springs.size() - startIndex == 0)
        {
            return CalculateSpecyficCombinations(springs, groups, combinationsMap, startIndex);
        }
        
        // check if there is possible 1 blank space 
        return springs[startIndex] == Damaged ? 0 : CalculateSpecyficCombinations(springs, groups, combinationsMap, startIndex + 1);
    }
    
    if (springs[startIndex] == Ok)
    {
        return 0;
    }

    return ConsumeGeyserGroup(springs, groups, combinationsMap, startIndex + 1, groupSize - 1);

}

size_t CalculateSpecyficCombinations( const std::vector<GeyserStatus>& springs
                                    , std::deque<size_t>& groups
                                    , std::map<std::pair<size_t, size_t>, size_t>& combinationsMap
                                    , size_t startIndex)
{
    if (combinationsMap.count(std::make_pair(startIndex, groups.size())))
    {
        return combinationsMap[std::make_pair(startIndex, groups.size())];
    }

    if (startIndex >= springs.size())
    {
        return groups.size() ? 0 : 1;
    }

    size_t retVal = 0;
    if (springs[startIndex] == Ok || springs[startIndex] == Unknown)
    {
        retVal = CalculateSpecyficCombinations(springs, groups, combinationsMap, startIndex+1);
    }
    
    if (springs[startIndex] == Damaged || springs[startIndex] == Unknown)
    {
        size_t groupSize = groups.front();
        groups.pop_front();
        retVal += ConsumeGeyserGroup(springs, groups, combinationsMap, startIndex, groupSize);
        groups.push_front(groupSize);
    }

    combinationsMap[std::make_pair(startIndex, groups.size())] = retVal;
    return retVal;
}

size_t CalculateCombinations(const std::vector<GeyserStatus>& springs, std::deque<size_t>& groups)
{
    std::map<std::pair<size_t, size_t>, size_t> combinationsMap; // key - start index, groups left; value - num combinations for given key

    CalculateSpecyficCombinations(springs, groups, combinationsMap, 0);

    return combinationsMap[std::make_pair(0, groups.size())];
}

size_t ParseLine(std::string line)
{
    std::vector<GeyserStatus> springs;
    std::deque<size_t> groups;
    const size_t spaceIdx = line.find(' ');

    for (size_t i = 0; i < spaceIdx; i++)
    {
        switch (line[i])
        {
        case '.': springs.push_back(Ok); break;
        case '#': springs.push_back(Damaged); break;
        case '?': springs.push_back(Unknown); break;  
        default:
            assert(0);
            break;
        }
    }

    line.erase(0, spaceIdx+1);
    while (line.length() > 0)
    {
        size_t pos = 0;
        groups.push_back(std::stoi(line, &pos));
        line.erase(0, pos+1);
    }
    
    return CalculateCombinations(springs, groups);
}

size_t SolveGame(const std::string& input)
{
    std::istringstream stream(input);
    std::string line;
    size_t result = 0;
    while (std::getline(stream, line))
    {
        unsigned subresult = ParseLine(line); 
        std::cout << std::endl << "The subresult is " << subresult << std::endl;
        result += subresult;
    }
    
    std::cout << std::endl << "The final result is " << result << std::endl;
    return result;
}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
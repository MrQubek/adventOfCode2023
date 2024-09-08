

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
#include <numeric>

enum GeyserStatus
{
    Ok,
    Damaged,
    Unknown
};

inline bool IsCurrentGeyserGroupValid
( const std::vector<GeyserStatus>& springs
, int currentIndex
, int currentGeyserGroupSize
)
{
    if (currentGeyserGroupSize < 0)
    {
        return true;
    }
    else if (currentGeyserGroupSize == 0)
    {
        return springs[currentIndex] != GeyserStatus::Damaged;
    }
    // currentGroup > 0
    return springs[currentIndex] != GeyserStatus::Ok;
}

size_t CalculateCombinations
    ( const std::vector<GeyserStatus>& springs
    , std::vector<int>& groups
    , int currentIndex
    , int currentGeyserGroupSize
    , int numValidSpringsLeft
    , int numUnknownSpringsLeft
    )
{
    if  (   groups.size() == 0
        &&  currentGeyserGroupSize == 0
        )
    {
        return numValidSpringsLeft == 0 ? 1 : 0;
    }
    
    if  (    currentIndex < 0
        ||  !IsCurrentGeyserGroupValid(springs, currentIndex, currentGeyserGroupSize)
        ||   (numUnknownSpringsLeft == 0 && numValidSpringsLeft == 0)
        )
    {
        return 0;
    }

    size_t retVal = 0;
    if (currentGeyserGroupSize > 0)
    {
        if (springs[currentIndex]==GeyserStatus::Damaged)
        {
            --numValidSpringsLeft;
        }
        else if (springs[currentIndex]==GeyserStatus::Unknown)
        {
            --numUnknownSpringsLeft;
        }
        retVal = CalculateCombinations(springs, groups, --currentIndex, currentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft);
    }
    else if (currentGeyserGroupSize == 0)
    {
        if (springs[currentIndex]==GeyserStatus::Unknown)
        {
            --numUnknownSpringsLeft;
        }
        retVal = CalculateCombinations(springs, groups, --currentIndex, currentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft);
    }
    else // if currentGeyserGroupSize < 0
    {
        if (springs[currentIndex] == GeyserStatus::Ok)
        {
            retVal = CalculateCombinations(springs, groups, --currentIndex, currentGeyserGroupSize, numValidSpringsLeft, numUnknownSpringsLeft);
        }
        else
        {
            if (springs[currentIndex] == GeyserStatus::Damaged)
            {
                --numValidSpringsLeft;
                currentGeyserGroupSize = groups.back();
                groups.pop_back();
                retVal = CalculateCombinations(springs, groups, --currentIndex, currentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft);
                groups.push_back(currentGeyserGroupSize);
            }
            else // GeyserStatus::Unknown
            {
                retVal = CalculateCombinations(springs, groups, currentIndex - 1, -1, numValidSpringsLeft, numUnknownSpringsLeft - 1);
                currentGeyserGroupSize = groups.back();
                groups.pop_back();                    
                retVal += CalculateCombinations(springs, groups, currentIndex - 1, currentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft - 1);
                groups.push_back(currentGeyserGroupSize);
            }
        }
    }
    return retVal;
}

size_t StartCalculations(const std::vector<GeyserStatus>& springs, std::vector<int>& groups)
{
    int numSprings = std::count_if(springs.begin(), springs.end(), [](GeyserStatus g){ return g == GeyserStatus::Damaged;});
    int numUnknown = std::count_if(springs.begin(), springs.end(), [](GeyserStatus g){ return g == GeyserStatus::Unknown;});
    return CalculateCombinations(springs, groups, springs.size()-1, -1, numSprings, numUnknown);
}

size_t ParseLine(std::string line)
{
    std::vector<GeyserStatus> oldSprings;
    std::vector<int> oldGroups;
    const size_t spaceIdx = line.find(' ');

    for (size_t i = 0; i < spaceIdx; i++)
    {
        switch (line[i])
        {
        case '.': oldSprings.push_back(Ok); break;
        case '#': oldSprings.push_back(Damaged); break;
        case '?': oldSprings.push_back(Unknown); break;  
        default:
            assert(0);
            break;
        }
    }

    std::vector<GeyserStatus> springs;
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < oldSprings.size(); j++)
        {
            springs.push_back(oldSprings[j]);
        }
        springs.push_back(Unknown);
    }
    springs.pop_back();
    

    line.erase(0, spaceIdx+1);
    while (line.length() > 0)
    {
        size_t pos = 0;
        oldGroups.push_back(std::stoi(line, &pos));
        line.erase(0, pos+1);
    }

    std::vector<int> groups;
    for (size_t i = 0; i < 5; i++)
    {
        for (const auto& oldGroup : oldGroups)
        {
            groups.push_back(oldGroup);
        }
    }    
    
    return StartCalculations(oldSprings, oldGroups);
}

size_t SolveGame(const std::string& input)
{
    std::istringstream stream(input);
    std::string line;
    size_t result = 0;
    while (std::getline(stream, line))
    {
        unsigned subresult = ParseLine(line); 
        std::cout << "The subresult is " << subresult << std::endl;
        result += subresult;
    }
    
    std::cout << std::endl << "The final result is " << result << std::endl;
    return result;
}

int main(){

    SolveGame(testInput);
    // SolveGame(gameInput);

    return 0;
}


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
, int32_t currentIndex
, int32_t currentGeyserGroupSize
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

struct TupleHash : public std::unary_function<std::tuple<int32_t, int32_t, int32_t>, std::size_t>
{
 std::size_t operator()(const std::tuple<int32_t, int32_t, int32_t>& k) const
 {
   return (size_t)std::get<0>(k) << 48 | (size_t)std::get<1>(k) << 24 | (size_t)std::get<2>(k);
 }
};

size_t CalculateCombinations
    ( const std::vector<GeyserStatus>& springs
    , std::vector<int32_t>& groups
    , int32_t currentIndex
    , int32_t currentGeyserGroupSize
    , int32_t numValidSpringsLeft
    , int32_t numUnknownSpringsLeft
    , std::unordered_map<std::tuple<int32_t, int32_t, int32_t>, size_t, TupleHash>& combinationsMap
    )
{
    assert(numValidSpringsLeft>=0);
    assert(numUnknownSpringsLeft>=0);
    assert(currentGeyserGroupSize>=-1);
    auto it = combinationsMap.find(std::make_tuple(groups.size(), currentIndex, currentGeyserGroupSize));
    if (it != combinationsMap.end())
    {
        return it->second;
    }
    
    if  (   groups.size() == 0
        &&  currentGeyserGroupSize <= 0
        )
    {
        auto res = combinationsMap.emplace(std::make_tuple(groups.size(), currentIndex, currentGeyserGroupSize), numValidSpringsLeft == 0);
        assert(res.second);
        return numValidSpringsLeft == 0;
    }
    
    if  (    currentIndex < 0
        ||  !IsCurrentGeyserGroupValid(springs, currentIndex, currentGeyserGroupSize)
        ||   (numUnknownSpringsLeft == 0 && numValidSpringsLeft == 0)
        )
    {
        auto res = combinationsMap.emplace(std::make_tuple(groups.size(), currentIndex, currentGeyserGroupSize), 0);
        assert(res.second);
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
        retVal = CalculateCombinations(springs, groups, currentIndex - 1, currentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft, combinationsMap);
    }
    else if (currentGeyserGroupSize == 0)
    {
        if (springs[currentIndex]==GeyserStatus::Unknown)
        {
            --numUnknownSpringsLeft;
        }
        retVal = CalculateCombinations(springs, groups, currentIndex - 1, currentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft, combinationsMap);
    }
    else // if currentGeyserGroupSize < 0
    {
        if (springs[currentIndex] == GeyserStatus::Ok)
        {
            retVal = CalculateCombinations(springs, groups, currentIndex - 1, currentGeyserGroupSize, numValidSpringsLeft, numUnknownSpringsLeft, combinationsMap);
        }
        else
        {
            if (springs[currentIndex] == GeyserStatus::Damaged)
            {
                int newCurrentGeyserGroupSize = groups.back();
                groups.pop_back();
                retVal = CalculateCombinations(springs, groups, currentIndex - 1, newCurrentGeyserGroupSize - 1, numValidSpringsLeft - 1, numUnknownSpringsLeft, combinationsMap);
                groups.push_back(newCurrentGeyserGroupSize);
            }
            else // GeyserStatus::Unknown
            {
                retVal = CalculateCombinations(springs, groups, currentIndex - 1, -1, numValidSpringsLeft, numUnknownSpringsLeft - 1, combinationsMap);
                int newCurrentGeyserGroupSize = groups.back();
                groups.pop_back();                    
                retVal += CalculateCombinations(springs, groups, currentIndex - 1, newCurrentGeyserGroupSize - 1, numValidSpringsLeft, numUnknownSpringsLeft - 1, combinationsMap);
                groups.push_back(newCurrentGeyserGroupSize);
            }
        }
    }
    auto result = combinationsMap.emplace(std::make_tuple(groups.size(), currentIndex, currentGeyserGroupSize), retVal);
    assert(result.second);
    return retVal;
}

size_t StartCalculations(const std::vector<GeyserStatus>& springs, std::vector<int32_t>& groups)
{
    int32_t numSprings = std::count_if(springs.begin(), springs.end(), [](GeyserStatus g){ return g == GeyserStatus::Damaged;});
    int32_t numUnknown = std::count_if(springs.begin(), springs.end(), [](GeyserStatus g){ return g == GeyserStatus::Unknown;});
    std::unordered_map<std::tuple<int32_t, int32_t, int32_t>, size_t, TupleHash> combinationsMap{};
    auto oryginalGroups = groups;
    size_t retVal = CalculateCombinations(springs, groups, springs.size()-1, -1, numSprings, numUnknown, combinationsMap);
    assert(oryginalGroups == groups);
    return retVal;
}

size_t ParseLine(std::string line)
{
    std::vector<GeyserStatus> oldSprings;
    std::vector<int32_t> oldGroups;
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

    std::vector<int32_t> groups;
    for (size_t i = 0; i < 5; i++)
    {
        for (const auto& oldGroup : oldGroups)
        {
            groups.push_back(oldGroup);
        }
    }    
    
    return StartCalculations(springs, groups);
}

size_t SolveGame(const std::string& input)
{
    std::istringstream stream(input);
    std::string line;
    size_t result = 0;
    while (std::getline(stream, line))
    {
        size_t subresult = ParseLine(line); 
        std::cout << "The subresult is " << subresult << std::endl;
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
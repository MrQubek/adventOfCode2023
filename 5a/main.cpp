

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

struct DestRange
{
    long long mStart;
    long long mLength;
};

struct SourceToDestMap
{
    std::string mDest;
    std::map<long long, DestRange> mMap;
};


struct State
{
    std::vector<long long> mSeeds;
    std::unordered_map<std::string, SourceToDestMap> mSourceToDest;
};

void ParseInput(std::string& input, State& state)
{
    std::istringstream stream(input);
    std::string line;

    std::getline(stream, line);

    line.erase(0, 7);
    size_t pos = 0;
    while (line.length())
    {
        state.mSeeds.push_back(stoll(line, &pos));
        line.erase(0, pos+1);
    }
    std::getline(stream, line);

    while (std::getline(stream, line)) {
        std::string source = line.substr(0, line.find_first_of('-'));
        line.erase(0, line.find_first_of('-')+1);
        std::string dest = line.substr(line.find_first_of('-') + 1, line.find_first_of(' ') - line.find_first_of('-') -1);

        SourceToDestMap& sourceToDest = state.mSourceToDest[source];
        sourceToDest.mDest = dest;

        while (std::getline(stream, line) && line != "")
        {
            // std::cout <<line<< std::endl;
            long long source = stoll(line, &pos);
            line.erase(0, pos+1);
            long long dest = stoll(line, &pos);
            line.erase(0, pos+1);
            long long length = stoll(line, &pos);
            sourceToDest.mMap[dest] = {source, length};
        }
    }
}

static const std::string mStartItem("seed");
static const std::string mStopItem("location");

long long GetLocation(std::unordered_map<std::string, SourceToDestMap> sourceToDest, long long seed)
{
    long long source = seed;
    std::string sourceName = mStartItem;

    do
    {
        for(auto const& [key, val] : sourceToDest[sourceName].mMap)
        {
            if(source < key)
            {
                break;
            }
            else if (key + val.mLength > source)
            {
                long long offset = source - key;
                source = val.mStart + offset;
                break;
            }
        }

        sourceName = sourceToDest[sourceName].mDest;
    } while (sourceName != mStopItem);
    
    return source;
}

void SolveGame(std::string& input)
{
    State state;

    ParseInput(input, state);

    long long minLocation = std::numeric_limits<long long>::max();

    for(auto seed : state.mSeeds)
    {
        auto location = GetLocation(state.mSourceToDest, seed);
        minLocation = location < minLocation ? location : minLocation;
    }

    std::cout << std::endl << "The result is " << minLocation << std::endl << std::endl;
}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
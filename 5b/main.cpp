

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
#include <future>
#include <thread>

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
    std::vector<DestRange> mSeeds;
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
        auto start = stoll(line, &pos);
        line.erase(0, pos+1);
        auto length = stoll(line, &pos);
        line.erase(0, pos+1);
        state.mSeeds.push_back({start, length});
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

long long GetMinLocationFromRange(const std::unordered_map<std::string, SourceToDestMap> sourceToDest, const long long startSeed, const long long numSeeds)
{
    std::vector<DestRange> sourceRanges, destRanges;
    sourceRanges.push_back({startSeed, numSeeds});
    std::string sourceName = mStartItem;

    do
    {
        auto itemRange = sourceRanges.begin();
        auto itemSourceStart =  itemRange->mStart;
        auto itemLength =       itemRange->mLength;

        auto& sourceMap = sourceToDest.at(sourceName).mMap;
        auto currSourceToDest = sourceMap.begin();

        while (itemRange != sourceRanges.end() && currSourceToDest != sourceMap.end())
        {
            const auto currSourceStart = currSourceToDest->first;
            const auto currDestStart = currSourceToDest->second.mStart;
            const auto currDestLength = currSourceToDest->second.mLength;

            if  (   itemSourceStart > currSourceStart + currDestLength
                ||  itemSourceStart + itemLength < currSourceStart
                )
            {
                currSourceToDest++;
            } else
            {
                if (itemSourceStart < currSourceStart)
                {
                    long long numItems = currSourceStart - itemSourceStart;
                    destRanges.push_back({itemSourceStart, numItems});
                    itemSourceStart = currSourceStart;
                    itemLength -= numItems;
                }

                auto currSourceEnd = currSourceStart + currDestLength;
                auto offset = itemSourceStart - currSourceStart;
                if (itemSourceStart + itemLength >= currSourceEnd)
                {
                    long long numItems = currDestLength - offset;
                    destRanges.push_back({currDestStart + offset, currDestLength - offset});
                    itemSourceStart = currSourceEnd;
                    itemLength = itemLength - numItems;
                    currSourceToDest++;
                } else
                {
                    destRanges.push_back({currDestStart + offset, itemLength});
                    itemRange++;
                    if(itemRange != sourceRanges.end())
                    {
                        itemSourceStart = itemRange->mStart;
                        itemLength = itemRange->mLength;
                    }
                }
            }
        }

        if (itemRange != sourceRanges.end())
        {
            destRanges.push_back({itemSourceStart, itemLength});
            itemRange++;
            while (itemRange != sourceRanges.end())
            {
                destRanges.push_back({itemRange->mStart, itemRange->mLength});
                itemRange++;
            }
        }
        
        sourceRanges.clear();
        sourceRanges.swap(destRanges);
        std::sort(sourceRanges.begin(), sourceRanges.end(), [](const DestRange& l, const DestRange& r)
        {
            return l.mStart < r.mStart;
        });

        sourceName = sourceToDest.at(sourceName).mDest;
    } while (sourceName != mStopItem);
    
    return sourceRanges[0].mStart;
}

// long long GetMinFromSeedRange(const DestRange& seedRange, const State& state)
// {
//     std::thread::id id = std::this_thread::get_id();
//     std::cout<<"Thread "<<id<<" starts"<<std::endl;
//     long long minLocation = std::numeric_limits<long long>::max();

//     auto location = GetLocations(state.mSourceToDest, seedRange.mStart, seedRange.mLength);
//     // minLocation = location < minLocation ? location : minLocation;

//     return location;
// }

void SolveGame(std::string& input)
{
    State state;

    ParseInput(input, state);

    std::vector<long long> minLocations;
    // std::vector<std::future<long long>> futures(state.mSeeds.size());

    for(int i = 0; i < state.mSeeds.size();i++)
    {
        minLocations.push_back(GetMinLocationFromRange(state.mSourceToDest, state.mSeeds[i].mStart, state.mSeeds[i].mLength));
    }

    // for(int i = 0; i < state.mSeeds.size();i++)
    // {
    //     futures[i] = std::async(std::launch::async, GetMinFromSeedRange, state.mSeeds[i], state);
    // }

    // std::vector<long long> mins;
    // for(int i = 0; i < state.mSeeds.size();i++)
    // {
    //     futures[i].wait();
    //     mins.push_back(futures[i].get());
    // }
    // std::cout << std::endl << "The result is " << *std::min_element(mins.begin(), mins.end()) << std::endl << std::endl;
    std::cout << std::endl << "The result is " << *std::min_element(minLocations.begin(), minLocations.end()) << std::endl << std::endl;
}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
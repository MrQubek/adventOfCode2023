

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
#include <queue>

struct Galaxy
{
    int posX;
    int posY;
    Galaxy(unsigned X, unsigned Y):posX(X), posY(Y){}
};

using EmptySpaces = std::vector<unsigned>;

std::vector<Galaxy> ParseInput(const std::string& input)
{
    std::vector<Galaxy> galaxies;

    std::istringstream stream(input);
    std::string line;
    std::getline(stream, line);

    std::vector<bool> IsFilledCol(line.size());

    unsigned rowIdx = 0;
    unsigned rowOffset = 0;
    do
    {
        bool rowEmpty = true;
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == '#')
            {
                rowEmpty = false;
                IsFilledCol[i] = true;
                galaxies.emplace_back(i, rowIdx + rowOffset);
            }
        }
     
        if (rowEmpty)
        {
            rowOffset++;
        }

        rowIdx++;
    }while (std::getline(stream, line));

    unsigned colOffset = 0;
    EmptySpaces emptyCols;
    for (size_t i = 0; i < IsFilledCol.size(); i++)
    {
        if (!IsFilledCol[i])
        {
            colOffset++;
        }
        emptyCols.push_back(colOffset);
    }

    std::transform(galaxies.begin(), galaxies.end(), galaxies.begin(), [&emptyCols = std::as_const(emptyCols)](const Galaxy& g){return Galaxy(g.posX + emptyCols[g.posX], g.posY);});

    return galaxies;
}

size_t SolveGame(const std::string& input)
{
    auto galaxies = ParseInput(input);
 
    size_t result{};
    for (size_t i = 0; i < galaxies.size()-1; i++)
    {
        for (size_t j = i+1; j < galaxies.size(); j++)
        {
            result += std::abs(galaxies[i].posX - galaxies[j].posX) + std::abs(galaxies[i].posY - galaxies[j].posY);
        }
    }
    
    std::cout << std::endl << "The result is " << result << std::endl;
    return result;
}

int main(){

    assert(SolveGame(testInput) == 374); 
    SolveGame(gameInput);

    return 0;
}
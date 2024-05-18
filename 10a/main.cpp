

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

void ReadInput(const std::string& input, std::vector<std::string> &map)
{
    std::istringstream stream(input);
    std::string line;
    while (std::getline(stream, line))
    {
        map.push_back(line);
    }
}

void FindStart(const std::vector<std::string> &map, unsigned& X, unsigned& Y)
{
    for (Y = 0; Y < map.size(); Y++)
    {
        size_t pos = map[Y].find('S');
        if (pos != std::string::npos)
        {
            X = pos;
            return;
        }
    }
    exit(-2);
}

enum class DirFrom
{
    N,
    S,
    W,
    E
};

struct Vec
{
    unsigned X;
    unsigned Y;
    DirFrom dir;

    Vec(unsigned X, unsigned Y, DirFrom d):X(X),Y(Y),dir(d){};

    bool operator ==(const Vec& other)
    {
        return X == other.X && Y == other.Y;
    }
};

void AdvanceRat(const std::vector<std::string> &map, Vec& rat)
{
    switch (map[rat.Y][rat.X])
    {
    case '|':
        if(rat.dir == DirFrom::S)
        {
            rat.Y = rat.Y - 1;
        }
        else
        {
            rat.Y = rat.Y + 1;
        }
        break;
    case '-': 
        if(rat.dir == DirFrom::E)
        {
            rat.X = rat.X - 1;
        }
        else
        {
            rat.X = rat.X + 1;
        }
        break;
    case 'L': 
        if(rat.dir == DirFrom::N)
        {
            rat.X = rat.X + 1;
            rat.dir = DirFrom::W;
        }
        else
        {
            rat.Y = rat.Y - 1;
            rat.dir = DirFrom::S;
        }
        break;
    case 'J': 
        if(rat.dir == DirFrom::N)
        {
            rat.X = rat.X - 1;
            rat.dir = DirFrom::E;
        }
        else
        {
            rat.Y = rat.Y - 1;
            rat.dir = DirFrom::S;
        }
        break;
    case '7': 
        if(rat.dir == DirFrom::S)
        {
            rat.X = rat.X - 1;
            rat.dir = DirFrom::E;
        }
        else
        {
            rat.Y = rat.Y + 1;
            rat.dir = DirFrom::N;
        }
        break;
    case 'F': 
        if(rat.dir == DirFrom::S)
        {
            rat.X = rat.X + 1;
            rat.dir = DirFrom::W;
        }
        else
        {
            rat.Y = rat.Y + 1;
            rat.dir = DirFrom::N;
        }
        break;
    
    default:
        std::cout<<map[rat.Y][rat.X]<<std::endl;
        exit(-1);
    }
}

std::vector<Vec> FindStartingPos(std::vector<std::string>& map, unsigned startX, unsigned startY)
{
    std::vector<Vec> rats;
    if (startX > 0)
    {
        if  (   map[startY][startX-1]=='-'
            ||  map[startY][startX-1]=='L'
            ||  map[startY][startX-1]=='F'
            )
        {
            rats.emplace_back(startX-1, startY, DirFrom::E);
        }
    }
    if (startX - 1 < map[0].size())
    {
        if  (   map[startY][startX+1]=='-'
            ||  map[startY][startX+1]=='J'
            ||  map[startY][startX+1]=='7'
            )
        {
            rats.emplace_back(startX+1, startY, DirFrom::W);
        }
    }
    if (startY > 0)
    {
        if  (   map[startY-1][startX]=='|'
            ||  map[startY-1][startX]=='F'
            ||  map[startY-1][startX]=='7'
            )
        {
            rats.emplace_back(startX, startY - 1, DirFrom::S);
        }
    }
    if (startY - 1 < map.size())
    {
        if  (   map[startY+1][startX]=='|'
            ||  map[startY+1][startX]=='L'
            ||  map[startY+1][startX]=='J'
            )
        {
            rats.emplace_back(startX, startY + 1, DirFrom::N);
        }
    }

    assert(rats.size() == 2);
    return rats;
}

void SolveGame(const std::string& input)
{
    std::vector<std::string> map;
    ReadInput(input, map);

    size_t numSteps = 1;
    unsigned startX, startY;
    FindStart(map, startX, startY);

    std::vector<Vec> rats = FindStartingPos(map, startX, startY);

    while (!(rats[0] == rats[1]))
    {
        AdvanceRat(map, rats[0]);
        AdvanceRat(map, rats[1]);
        numSteps++;
    }
    
    std::cout << std::endl << "The result is " << numSteps << std::endl;
}

int main(){

    SolveGame(testInput1);
    SolveGame(gameInput);

    return 0;
}
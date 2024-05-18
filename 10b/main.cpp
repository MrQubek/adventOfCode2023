

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

struct MapPoint
{
    char terrain;
    bool visited;

    MapPoint(char c):terrain(c),visited(false){};
};

struct Vec
{
    unsigned X;
    unsigned Y;
    DirFrom dir;

    Vec(unsigned X, unsigned Y, DirFrom d = DirFrom::N):X(X),Y(Y),dir(d){};

    bool operator ==(const Vec& other)
    {
        return X == other.X && Y == other.Y;
    }
};

void AdvanceRat(std::vector<std::vector<MapPoint>> &map, Vec& rat)
{
    unsigned oldX = rat.X;
    unsigned oldY = rat.Y;
    switch (map[rat.Y][rat.X].terrain)
    {
    case '|':
        if(rat.dir == DirFrom::S)
        {
            map[rat.Y-1][rat.X].terrain = 'X';
            rat.Y = rat.Y - 2;
        }
        else
        {
            map[rat.Y+1][rat.X].terrain = 'X';
            rat.Y = rat.Y + 2;
        }
        break;
    case '-': 
        if(rat.dir == DirFrom::E)
        {
            map[rat.Y][rat.X-1].terrain = 'X';
            rat.X = rat.X - 2;
        }
        else
        {
            map[rat.Y][rat.X+1].terrain = 'X';
            rat.X = rat.X + 2;
        }
        break;
    case 'L': 
        if(rat.dir == DirFrom::N)
        {
            map[rat.Y][rat.X+1].terrain = 'X';
            rat.X = rat.X + 2;
            rat.dir = DirFrom::W;
        }
        else
        {
            map[rat.Y-1][rat.X].terrain = 'X';
            rat.Y = rat.Y - 2;
            rat.dir = DirFrom::S;
        }
        break;
    case 'J': 
        if(rat.dir == DirFrom::N)
        {
            map[rat.Y][rat.X-1].terrain = 'X';
            rat.X = rat.X - 2;
            rat.dir = DirFrom::E;
        }
        else
        {
            map[rat.Y-1][rat.X].terrain = 'X';
            rat.Y = rat.Y - 2;
            rat.dir = DirFrom::S;
        }
        break;
    case '7': 
        if(rat.dir == DirFrom::S)
        {
            map[rat.Y][rat.X-1].terrain = 'X';
            rat.X = rat.X - 2;
            rat.dir = DirFrom::E;
        }
        else
        {
            map[rat.Y+1][rat.X].terrain = 'X';
            rat.Y = rat.Y + 2;
            rat.dir = DirFrom::N;
        }
        break;
    case 'F': 
        if(rat.dir == DirFrom::S)
        {
            map[rat.Y][rat.X+1].terrain = 'X';
            rat.X = rat.X + 2;
            rat.dir = DirFrom::W;
        }
        else
        {
            map[rat.Y+1][rat.X].terrain = 'X';
            rat.Y = rat.Y + 2;
            rat.dir = DirFrom::N;
        }
        break;
    
    default:
        // std::cout<<map[rat.Y][rat.X]<<std::endl;
        exit(-1);
    }
    map[oldY][oldX].terrain = 'X';
}

std::vector<Vec> FindStartingPos(std::vector<std::vector<MapPoint>>& map, unsigned startX, unsigned startY)
{
    std::vector<Vec> rats;
    if (startX > 1)
    {
        if  (   map[startY][startX-2].terrain=='-'
            ||  map[startY][startX-2].terrain=='L'
            ||  map[startY][startX-2].terrain=='F'
            )
        {
            map[startY][startX-1].terrain = 'X';
            rats.emplace_back(startX-2, startY, DirFrom::E);
        }
    }
    if (startX == 1 || startX - 2 < map[0].size())
    {
        if  (   map[startY][startX+2].terrain=='-'
            ||  map[startY][startX+2].terrain=='J'
            ||  map[startY][startX+2].terrain=='7'
            )
        {
            map[startY][startX+1].terrain = 'X';
            rats.emplace_back(startX+2, startY, DirFrom::W);
        }
    }
    if (startY > 1)
    {
        if  (   map[startY-2][startX].terrain=='|'
            ||  map[startY-2][startX].terrain=='F'
            ||  map[startY-2][startX].terrain=='7'
            )
        {
            map[startY-1][startX].terrain = 'X';
            rats.emplace_back(startX, startY - 2, DirFrom::S);
        }
    }
    if (startY == 1 || startY - 2 < map.size())
    {
        if  (   map[startY+2][startX].terrain=='|'
            ||  map[startY+2][startX].terrain=='L'
            ||  map[startY+2][startX].terrain=='J'
            )
        {
            map[startY+1][startX].terrain = 'X';
            rats.emplace_back(startX, startY + 2, DirFrom::N);
        }
    }

    assert(rats.size() == 2);
    return rats;
}

std::vector<std::vector<MapPoint>> IncreaseMapResolution(const std::vector<std::string>& map)
{
    std::vector<std::vector<MapPoint>> enhancedMap;
    enhancedMap.emplace_back(map[0].length()*2 + 1, MapPoint('*'));
    size_t rowIdx = 1;
    for (const auto line : map)
    {
        size_t colIdx = 1;
        enhancedMap.emplace_back(map[0].length()*2 + 1, MapPoint('*'));
        for (const char c : line)
        {
            enhancedMap[rowIdx][colIdx] = MapPoint(c);
            colIdx += 2;
        }
        enhancedMap.emplace_back(map[0].length()*2 + 1, MapPoint('*'));
        rowIdx += 2;
    }
    return enhancedMap;
}

void PrintEnhancedMap(const std::vector<std::vector<MapPoint>>& map)
{
    for (const auto & row : map)
    {
        for (const auto & col : row)
        {
            if (col.visited && col.terrain!='X')
                std::cout<<'+';
            else
                std::cout << col.terrain;
        }
        std::cout << std::endl;
    }
}

void VisitPlaces(std::vector<std::vector<MapPoint>>& map)
{
    std::queue<Vec> pointsQueue;
    pointsQueue.emplace(0,0);

    while (!pointsQueue.empty())
    {
        Vec point = pointsQueue.front();
        pointsQueue.pop();

        if (!map[point.Y][point.X].visited)
        {
            map[point.Y][point.X].visited = true;
            if (map[point.Y][point.X].terrain!='X')
            {
                if (point.Y>0)
                    pointsQueue.emplace(point.X,point.Y-1);
                if (point.Y == 0 || point.Y+1 < map.size())
                    pointsQueue.emplace(point.X,point.Y+1);
                if (point.X>0)
                    pointsQueue.emplace(point.X-1,point.Y);
                if (point.X == 0 || point.X+1 < map[0].size())
                    pointsQueue.emplace(point.X+1,point.Y);
            }
        // PrintEnhancedMap(map);
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
}

size_t SolveGame(const std::string& input)
{
    std::vector<std::string> map;
    ReadInput(input, map);

    auto enhancedMap = IncreaseMapResolution(map);
    PrintEnhancedMap(enhancedMap);

    unsigned startX, startY;
    FindStart(map, startX, startY);
    std::vector<Vec> rats = FindStartingPos(enhancedMap, startX*2+1, startY*2+1);
    enhancedMap[startY*2+1][startX*2+1].terrain = 'X';
    while (!(rats[0] == rats[1]))
    {
        AdvanceRat(enhancedMap, rats[0]);
        AdvanceRat(enhancedMap, rats[1]);
        // PrintEnhancedMap(enhancedMap);
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    enhancedMap[rats[0].Y][rats[0].X].terrain = 'X';

    VisitPlaces(enhancedMap);
    
    size_t numGround = 0;
    for (auto & row : enhancedMap)
    {
        for (auto & point : row )
        {
            if  (   !point.visited 
                &&  (   point.terrain != 'X'
                    &&  point.terrain != '*'   
    
                    ))
            {
                numGround++;
            }
        }
    }
    
    std::cout << std::endl << "The result is " << numGround << std::endl;
    return numGround;
}

int main(){

    assert(SolveGame(testInput[0]) == 4);
    assert(SolveGame(testInput[1]) == 8); 
    assert(SolveGame(testInput[2]) == 10);    
    SolveGame(gameInput);

    return 0;
}
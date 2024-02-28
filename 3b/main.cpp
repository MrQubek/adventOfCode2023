

#include "input.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cctype>

bool IsStar(char c)
{
    return c == '*';
}

struct Gear
{
    int x, value;
};


Gear GetGear(const std::vector<std::vector<char>>& inputMatrix, int x, const int y)
{
    Gear g{x, 0};
    while (x >= 0 && isdigit(inputMatrix[y][x]))
    {
        g.x = x;
        x--;
    }
    g.value = std::stoi(std::string(&inputMatrix[y][g.x]));

    return g;
}

std::vector<Gear> GetGearsFromRow(const std::vector<std::vector<char>>& inputMatrix, const int x, const int y, bool center)
{
    std::vector<Gear> gearsFound;

    if(y >= 0 && y < inputMatrix.size())
    {
        if(x - 1 >=0 && isdigit(inputMatrix[y][x-1]))
        {
            gearsFound.push_back(GetGear(inputMatrix, x-1, y));
        }
        if(!center && gearsFound.size()==0 && isdigit(inputMatrix[y][x]))
        {
            gearsFound.push_back(GetGear(inputMatrix, x, y));
        }
        if(x + 1 < inputMatrix[y].size() && !isdigit(inputMatrix[y][x]) && isdigit(inputMatrix[y][x+1]))
        {
            gearsFound.push_back(GetGear(inputMatrix, x+1, y));
        }
    }
    return gearsFound;
}

long long CalculateGearRatio(const std::vector<std::vector<char>>& inputMatrix, const int x, const int y)
{
    std::vector<Gear> gearsFound;

    for (size_t i = y-1; i <= y+1; i++)
    {
        auto g = GetGearsFromRow(inputMatrix, x, i, i == y);
        gearsFound.insert(gearsFound.end(), g.begin(), g.end());
    }
    
    return gearsFound.size() == 2 ? gearsFound[0].value*gearsFound[1].value : 0;
}

long long CalculateResult(const std::vector<std::vector<char>>& inputMatrix)
{
    int inputWidth = inputMatrix[0].size();
    int inputLength = inputMatrix.size();
    

    long long result = 0;
    for(int i = 0; i< inputLength; i++)
    {
        for(int j = 0; j < inputWidth; j++)
        {
            if(IsStar(inputMatrix[i][j]))
            {
                result += CalculateGearRatio(inputMatrix, j, i);                
            }
        }
    }

    return result;
}

void SolveGame(std::string& input)
{
    std::vector<std::vector<char>> inputMatrix;
    
    std::istringstream stream(input);
    std::string line;    
    while (std::getline(stream, line)) {
        inputMatrix.push_back(std::vector<char>(line.length()));
        memcpy(&(inputMatrix.back()[0]), line.c_str(), line.length());
        // std::cout << line << std::endl;
    }

    auto result = CalculateResult(inputMatrix);
    std::cout << std::endl << "The result is result " << result << std::endl << std::endl;

}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
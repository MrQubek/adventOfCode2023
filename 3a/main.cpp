

#include "input.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cctype>


bool IsSpecial(const std::vector<std::vector<char>>& inputMatrix, int x, int y)
{
    return !(isdigit(inputMatrix[y][x]) || inputMatrix[y][x] == '.');
}

bool BordersSpecialSign(const std::vector<std::vector<char>>& inputMatrix,const int rowIdx,const int startIdx,const size_t length)
{
    int inputWidth = inputMatrix[0].size();
    int inputLength = inputMatrix.size();

    if(rowIdx - 1 >=0)
    {
        for(int i = startIdx - 1; i < startIdx + length + 1; i++)
        {
            if(i < 0 || i >= inputWidth) continue;
            if (IsSpecial(inputMatrix,i , rowIdx - 1))
            {
                return true;
            }
            
        }
    }

    if (startIdx - 1 > 0 && IsSpecial(inputMatrix, startIdx - 1 , rowIdx)) return true;
    if (startIdx + length < inputWidth && IsSpecial(inputMatrix, startIdx + length , rowIdx)) return true;

    if(rowIdx + 1 < inputLength)
    {
        for(int i = startIdx - 1; i < (int)(startIdx + length + 1); i++)
        {
            if(i < 0 || i >= inputWidth) continue;
            if (IsSpecial(inputMatrix,i , rowIdx + 1))
            {
                return true;
            }
            
        }
    }
    
    return false;
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
            if(isdigit(inputMatrix[i][j]))
            {
                int start = j;
                size_t length = 0;
                int val = std::stoi(std::string(&inputMatrix[i][start]), &length);

                if (BordersSpecialSign(inputMatrix, i, j, length))
                {
                    result+=val;
                }

                j = j+length;
                
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
        std::cout << line << std::endl;
    }

    auto result = CalculateResult(inputMatrix);
    std::cout << std::endl << "The result is result " << result << std::endl << std::endl;

}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
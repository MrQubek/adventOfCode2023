

#include "input.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <cctype>
#include <set>
#include <algorithm>

int CalculateSingleGame(std::string str)
{
    str.erase(0, str.find_first_of(':')+2);

    std::set<int> winningNumbers;

    while (str[0]!='|')
    {
        size_t pos = 0;
        winningNumbers.insert(std::stoi(str, &pos));
        str.erase(0, pos+1);
    }
    
    str.erase(0, 2);
    std::set<int> ownNumbers;

    while (str.length())
    {
        size_t pos = 0;
        ownNumbers.insert(std::stoi(str, &pos));
        str.erase(0, pos);
    }

    int numWins = 0;

    std::for_each(ownNumbers.begin(), ownNumbers.end(), [&](int number){
        numWins+=winningNumbers.count(number);
    });

    return numWins == 0 ? 0 : 1 << (numWins-1);
}

void SolveGame(std::string& input)
{
    std::vector<std::vector<char>> inputMatrix;
    
    std::istringstream stream(input);
    std::string line;
    long long result = 0; 
    while (std::getline(stream, line)) {
        result += CalculateSingleGame(line);
    }
    std::cout << std::endl << "The result is " << result << std::endl << std::endl;
}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
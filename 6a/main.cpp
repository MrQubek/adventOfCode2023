

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

struct State
{
    std::vector<int> time;
    std::vector<int> distanceToBeat;
};

// generic solution
template <class T>
int numDigits(T number)
{
    int digits = 0;
    if (number < 0) digits = 1; // remove this line if '-' counts as a digit
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

void ParseInput(const std::string& input, State& state)
{
    std::istringstream stream(input);
    std::string line;
    long long result = 0; 

    std::getline(stream, line);
    line.erase(0, line.find_first_of(':')+1);
    while (line.length())
    {
        size_t pos = 0;
        state.time.push_back(std::stoi(line, &pos));
        line.erase(0, pos+1);
    }
    
    std::getline(stream, line);
    line.erase(0, line.find_first_of(':')+1);
    while (line.length())
    {
        size_t pos = 0;
        state.distanceToBeat.push_back(std::stoi(line, &pos));
        line.erase(0, pos+1);
    } 

    return;
}

int CalculateGame(const int time, const int timeToBeat)
{
    int delta = time *time - 4*timeToBeat;

    if(delta < 0 )
    {
        std::cerr << "Delta less than zero!"<<std::endl;
        abort();
    }

    float sqrtDelta = sqrt(delta);
    float x1f = (static_cast<float>(time) - sqrtDelta)/2;
    float x2f = (static_cast<float>(time) + sqrtDelta)/2;

    int x1 = ceil(x1f) == x1f ? ceil(x1f) + 1 : ceil(x1f);
    int x2 = floor(x2f)== x2f ? floor(x2f) - 1 : floor(x2f);
    
    int result = x2-x1 + 1;
    return result;
}

void SolveGame(std::string& input)
{
    State state;
    ParseInput(input, state);

    long int result = 1;

    for(int i = 0; i< state.distanceToBeat.size();i++)
    {
        result *= CalculateGame(state.time[i], state.distanceToBeat[i]);
    }

    std::cout << std::endl << "The result is " << result << std::endl << std::endl;
}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
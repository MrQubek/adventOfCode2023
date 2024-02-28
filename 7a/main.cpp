

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

enum class HandType
{
    FiveOfKind,
    FourOfKind,
    FullHouse,
    ThreeOfKind,
    TwoPair,
    OnePair,
    HighCard
};

struct Hand
{
    std::array<int, 5> cards;
    HandType type;
    size_t bid;
};

int ReadCardValue(char c)
{
    if (isdigit(c))
    {
        return c - '0';
    }

    switch (c)
    {
    case 'A': return 14;
    case 'K': return 13;
    case 'Q': return 12;
    case 'J': return 11;
    case 'T': return 10;
    default:
        std::cerr << "wrong conversion to int";
        abort();
        return 0;
    }
}

HandType GetHandType(const Hand & h)
{
    std::unordered_map<int, int> cardsMap;
    for( const int card : h.cards)
    {
        ++cardsMap[card];
    }

    std::vector<int> cardsCounts(5);
    std::transform(cardsMap.begin(), cardsMap.end(), std::back_inserter(cardsCounts), [](auto &kv){ return kv.second;});
    std::sort(cardsCounts.begin(), cardsCounts.end(), std::greater<int>());
    switch (cardsCounts[0])
    {
    case 5: return HandType::FiveOfKind;
    case 4: return HandType::FourOfKind;
    case 3: return cardsCounts[1]==2 ? HandType::FullHouse : HandType::ThreeOfKind;
    case 2: return cardsCounts[1]==2 ? HandType::TwoPair : HandType::OnePair;
    case 1: return HandType::HighCard;
    default:
        std::cerr << "wrong conversion to hand type";
        abort();
        return HandType::FiveOfKind;
    }
}

void ReadHands(const std::string& input, std::vector<Hand>& hands)
{
    std::istringstream stream(input);
    std::string line;
    while (std::getline(stream, line)) {
        Hand h{};
        for (size_t i = 0; i < 5; i++)
        {
            h.cards[i] = ReadCardValue(line[i]);
        }
        h.bid = std::stoi(line.substr(5));
        h.type = GetHandType(h);
        hands.push_back(h);
    }
}

void SolveGame(const std::string& input)
{
    std::vector<Hand> hands;
    ReadHands(input, hands);

    std::sort(hands.begin(), hands.end(),[](const Hand& hl, const Hand& hr)
    {
        if (hl.type == hr.type)
        {
            for (size_t i = 0; i < 5; i++)
            {
                if (hl.cards[i] != hr.cards[i])
                {
                    return hl.cards[i] < hr.cards[i];
                }
            }  
        }
        else
        {
            return hl.type > hr.type;
        }
        return false;
    });

    long long result = 0;
    for(int i = 1; i<= hands.size();i++)
    {
        result += i * hands[i-1].bid;
    }

    std::cout << std::endl << "The result is " << result << std::endl << std::endl;
}

int main(){

    SolveGame(testInput);
    SolveGame(gameInput);

    return 0;
}
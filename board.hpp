#pragma once
#include <vector> 
#include "player.hpp"
#include <array>

class fighting {
private:
    Player& player;
    std::array<std::array<int, 10>, 10> grid{};
    std::array<int, 2> krd;
public:
    fighting(Player& p, int x, int y) : player(p), krd{x, y} {}
    
};
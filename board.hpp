#pragma once
#include <vector>    
#include "player.hpp"
#include <array>
#include <algorithm>

class Board {        
private:
    Player& enemyPlayer;
    std::array<std::array<int, 10>, 10> gridForFight{};
    std::vector<std::array<int, 3>> lastdif{};
    void markCell(int x, int y, int state) {
        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            gridForFight[x][y] = state;
            lastdif.push_back({ x, y, state });
        }
    }

    void ifkilled(int x, int y, const Player& enemy) {
        std::vector<std::array<int, 2>> ship;

        findShipCells(x, y, enemy, ship);

        if (ship.empty()) return;

        // Находим границы корабля
        int min_x = 10, max_x = -1, min_y = 10, max_y = -1;
        for (const auto& cell : ship) {
            min_x = std::min(min_x, cell[0]);
            max_x = std::max(max_x, cell[0]);
            min_y = std::min(min_y, cell[1]);
            max_y = std::max(max_y, cell[1]);
        }

        for (int ix = min_x - 1; ix <= max_x + 1; ++ix) {
            for (int iy = min_y - 1; iy <= max_y + 1; ++iy) {
                if (ix >= 0 && ix < 10 && iy >= 0 && iy < 10) {
                    if (gridForFight[ix][iy] == 0) {
                        markCell(ix, iy, -1);
                    }
                }
            }
        }
        for (const auto& cell : ship) {
            markCell(cell[0], cell[1], -2); 
        }
    }

    void findShipCells(int x, int y, const Player& enemy, std::vector<std::array<int, 2>>& ship) {
        if (x < 0 || x >= 10 || y < 0 || y >= 10) return;
        if (enemy.showcell(x, y) != -2) return;
        for (const auto& cell : ship) {
            if (cell[0] == x && cell[1] == y) return;
        }

        ship.push_back({ x, y });

        findShipCells(x + 1, y, enemy, ship);
        findShipCells(x - 1, y, enemy, ship);
        findShipCells(x, y + 1, enemy, ship);
        findShipCells(x, y - 1, enemy, ship);
    }
    void updateVisibleGrid(int x, int y, const std::string& result) {
        lastdif = {};
        if (result == "missed") {
            markCell(x, y, -1);
        }
        if (result == "hitted") {
            markCell(x, y, -2);
        }
        if (result == "killed") {
            markCell(x, y, -2);
            ifkilled(x, y, enemyPlayer);
        }
    }
public:
    std::array<std::array<int, 10>, 10> Grid() const {
        return gridForFight;
    }
    Board() = default;
    Board& operator=(const Board&) = delete;
    Board(Player& enemy) : enemyPlayer(enemy) {}

    std::string MakeAttack(int x, int y) {
        std::string attack = enemyPlayer.fire(x, y);
        updateVisibleGrid(x, y, attack);
        return attack;
    }
    void getEnemy(Player& enemy) {
        enemyPlayer = enemy;
    }
    std::vector<std::array<int, 3>> showdif() {
        auto result = lastdif;
        lastdif.clear();
        return result;
    }
    int showcell(int x, int y) const { return gridForFight[x][y]; }
    bool iswin() const {
        return enemyPlayer.getShipsAlive() == 0;
    }
};
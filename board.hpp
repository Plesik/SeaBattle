#pragma once
#include <vector>    
#include "player.hpp"
#include <array>
#include <algorithm>

class Board {        //white = 0 not hitted, brown = -3 killed ship, blue =  -2 hitted in empty(помеченные), grey =  -1 hitted
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
    //если корабль убит, то в lastdif добавляются клетки вокруг него как помеченные
    void ifkilled(int x, int y, const Player& enemy) {
        std::vector<std::array<int, 2>> ship{};
        ship.push_back({ x, y });
        for (int i = 1; i < 4; i++) { //добавляем клетки убитого корабля в ship и изменяем grid
            bool u1 = true, u2 = true, u3 = true, u4 = true;
            if (x + i < 10) {
                if (enemy.showcell(x + i, y) == -1 && u1) {
                    ship.push_back({ x + i, y });
                }
                else {
                    u1 = false;
                }
            }
            if (x - i >= 0) {
                if (enemy.showcell(x - i, y) == -1 && u2) {
                    ship.push_back({ x - i, y });
                }
                else {
                    u2 = false;
                }
            }
            if (y + i < 10) {
                if (enemy.showcell(x, y + i) == -1 && u3) {
                    ship.push_back({ x, y + i });
                }
                else {
                    u3 = false;
                }
            }
            if (y - i >= 0) {
                if (enemy.showcell(x, y - i) == -1 && u4) {
                    ship.push_back({ x, y - i });
                }
                else {
                    u4 = false;
                }
            }
        }
        int min_x = 10, max_x = 0, min_y = 10, max_y = 0;
        for (const auto& cell : ship) {
            min_x = std::min(min_x, cell[0]);
            max_x = std::max(max_x, cell[0]);
            min_y = std::min(min_y, cell[1]);
            max_y = std::max(max_y, cell[1]);
        }

        for (int ix = min_x - 1; ix <= max_x + 1; ++ix) {
            for (int iy = min_y - 1; iy <= max_y + 1; ++iy) {
                if (ix < 0 || ix >= 10 || iy < 0 || iy >= 10) continue;
                if (gridForFight[ix][iy] == 0) {
                    markCell(ix, iy, -2);
                }
            }
        }

    }
    void updateVisibleGrid(int x, int y, const std::string& result) {
        lastdif = {}; //очищаем массив с изменениями
        if (result == "missed") {
            gridForFight[x][y] = -2; // Промах
            markCell(x, y, -2);
        }
        if (result == "hitted") {
            gridForFight[x][y] = -1;  // Попадание
            markCell(x, y, -1);
        }
        if (result == "killed") {
            markCell(x, y, -1);
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
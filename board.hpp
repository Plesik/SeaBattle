#pragma once
#include <vector>    
#include "player.hpp"
#include <array>
#include <algorithm>

class Board {        //white = 0 not hitted, blackgrey = -2 killed ship, . =  1 hitted in empty(помеченные), grey =  -1 hitted
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
        std::sort(ship.begin(), ship.end());
        for (auto cell : ship) {
            markCell(cell[0], cell[1], -2); //помечаем клетки убитого корабля
        }
        // Пометить клетки вокруг
        int min_x = ship[0][0], max_x = ship.back()[0];
        int min_y = ship[0][1], max_y = ship.back()[1];

        for (int ix = min_x - 1; ix <= max_x + 1; ++ix) {
            for (int iy = min_y - 1; iy <= max_y + 1; ++iy) {
                if (ix < 0 || ix >= 10 || iy < 0 || iy >= 10) continue;
                if (gridForFight[ix][iy] == 0) {
                    markCell(ix, iy, 1);
                }
            }
        }

    }
    void updateVisibleGrid(int x, int y, const std::string& result) {
        lastdif = {}; //очищаем массив с изменениями
        if (result == "missed") {
            gridForFight[y][x] = -2; // Промах
            lastdif.push_back({ x, y, 1 });
        }
        if (result == "hitted") {
            gridForFight[y][x] = -1;  // Попадание
            lastdif.push_back({ x, y, -1 });
        }
        if (result == "killed") {
            ifkilled(x, y, enemyPlayer);
        }
    }
public:
    std::array<std::array<int, 10>, 10> showdifGrid() const {
        return gridForFight;
    }
    Board(Player& enemy) : enemyPlayer(enemy) {}
    void MakeAttack(int x, int y) {
        std::string attack = enemyPlayer.fire(x, y);
        updateVisibleGrid(x, y, attack);
    }
    const std::vector<std::array<int, 3>> showdif() { return lastdif; }
    int showcell(int x, int y) const { return gridForFight[x][y]; }
};
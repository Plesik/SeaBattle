#pragma once
#include <array>
#include <iostream>

class Player {
private:
    std::array<std::array<int, 10>, 10> grid{}; // Поле 10x10 (инициализировано нулями)...
    int shipsAlive = 10;                         // Счетчик кораблей

public:
    // Добавляет корабль в указанную клетку
    void addShip(int x, int y) {
        grid[y][x] = 1;
        shipsAlive++;
    }

    std::string fire(int x, int y) {
        if (grid[x][y] == 1) {
            bool hasAliveParts = false;
            if (x > 0 && grid[y][x - 1] == 1) hasAliveParts = true;
            if (x < 9 && grid[y][x + 1] == 1) hasAliveParts = true;
            if (y > 0 && grid[y - 1][x] == 1) hasAliveParts = true;
            if (y < 9 && grid[y + 1][x] == 1) hasAliveParts = true;
            grid[x][y] = -1;
            if (!hasAliveParts) {
                shipsAlive--;
                return "killed";
            }
            else {
                return "hitted";
            }
        }
        else {
            grid[x][y] = -2;
            return "missed";
        }
    }

    // Отображает текущее состояние поля (для отладки)
    void displayGrid() const {
        std::cout << "  ";
        for (int i = 0; i < 10; ++i) std::cout << i << " ";
        std::cout << "\n";

        for (int y = 0; y < 10; ++y) {
            std::cout << y << " ";
            for (int x = 0; x < 10; ++x) {
                std::cout << grid[y][x] << " ";
            }
            std::cout << "\n";
        }
    }
    int showcell(int x, int y) { return grid[x][y]; }

    // Возвращает количество живых кораблей
    int getShipsAlive() const { return shipsAlive; }    
};
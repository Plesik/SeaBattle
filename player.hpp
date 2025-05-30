#pragma once
#include <array>
#include <iostream>

class Player { //����� �������� �� ��������� � ���� �� ������� �������������� �������, 
private:
    std::array<std::array<int, 12>, 12> grid{}; // ���� 12x12 (���������������� ������) ������� ������ ������ � ���� 10*10 ������
    int shipsAlive = 10;// ������� ��������
    std::array<int, 3> laststep{}; //{x, y, 1} - ������ � ������������ [x, y] ���� ������ ������� � �������, ���� -1 �� ��� ����� �� ������ ������
    bool checkalive(const std::array<std::array<int, 12>, 12>& grid, int x, int y, int dx, int dy) {
        if (grid[x + dx][y + dy] == 1) { return true; }
        if (grid[x + dx][y + dy] == -1) {
            if (grid[x + 2 * dx][y + 2 * dy] == 1) { return true; }
            if (grid[x + 2 * dx][y + 2 * dy] == -1) {
                if (grid[x + 3 * dx][y + 3 * dy] == 1) { return true; }
            }
        }
        return false;
    }
public:
    Player() = default;
    // ��������� �������(����� �������) � ��������� ������
    bool addShip(int xl, int yl) {
        int x = xl + 1;
        int y = yl + 1;
        bool usl = true;
        if (grid[x][y] != 1) {
            if (x + 1 <= 11 && y + 1 <= 11) {
                if (grid[x + 1][y + 1] == 1) { usl = false; }
            }
            if (x - 1 >= 1 && y + 1 <= 11) {
                if (grid[x - 1][y + 1] == 1) { usl = false; }
            }
            if (x + 1 <= 11 && y - 1 >= 1) {
                if (grid[x + 1][y - 1] == 1) { usl = false; }
            }
            if (x - 1 >= 1 && y - 1 >= 1) {
                if (grid[x - 1][y - 1] == 1) { usl = false; }
            }
            if (usl) {
                grid[x][y] = 1;
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    std::string fire(int xl, int yl) { //����� �� ��������� ������
        int x = xl + 1;
        int y = yl + 1;
        if (grid[x][y] == 1) {
            grid[x][y] = -1;
            laststep = { xl, yl, 1 };
            if (checkalive(grid, x, y, +1, 0) || checkalive(grid, x, y, -1, 0) || checkalive(grid, x, y, 0, +1) || checkalive(grid, x, y, 0, -1)) {
                return "hitted";
            }
            else {
                shipsAlive--;
                return "killed";
            }
        }
        else {
            grid[x][y] = -2;
            laststep = { xl, yl, -1 };
            return "missed";
        }
    }
    int showcell(int x, int y) const { return grid[x + 1][y + 1]; }

    const std::array<int, 3>& LastStep() const { return laststep; }

    const int getShipsAlive() const { return shipsAlive; }
};
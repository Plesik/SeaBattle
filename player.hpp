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
    // ��������� �������(����� �������) � ��������� ������
    void addShip(int x, int y) {
        grid[y][x] = 1;
    }

    std::string fire(int x, int y) { //����� �� ��������� ������
        if (grid[x+1][y+1] == 1) {
            grid[x+1][y+1] = -1;
            laststep = { x, y, 1 };
            if (checkalive(grid, x+1, y+1, +1, 0) || checkalive(grid, x+1, y+1, -1, 0) || checkalive(grid, x+1, y+1, 0, +1) || checkalive(grid, x+1, y+1, 0, -1)) {
                return "hitted";
            }
            else {
                shipsAlive--;
                return "killed";
            }
        }
        else {
            grid[x+1][y+1] = -2;
            laststep = { x, y, -1 };
            return "missed";
        }
    }
    int showcell(int x, int y) const { return grid[x+1][y+1]; }

    const std::array<int, 3>& LastStep() const { return laststep; }

    const int getShipsAlive() const { return shipsAlive; }    
};
#pragma once
#include <array>
#include <iostream>
#include <vector>

class Player { //класс отвечает за попадани€ в поле на котором расстановленны корабли, 
private:
    std::array<int, 4> countships = { 4, 3, 2, 1 };
    std::vector<std::array<int, 2>> shipnow{};
    std::vector<std::array<int, 2>> cellsfornext{};
    std::array<std::array<int, 10>, 10> grid{}; // ѕоле 12x12 (инициализировано нул€ми) корабли сто€ть только в поле 10*10 внутри
    std::vector<std::array<int, 3>> difgrid{};
    int shipsAlive = 10;// —четчик кораблей
    std::array<int, 3> laststep{ 0, 0, 0 }; //{x, y, 1} - клетка с координатами [x, y] была частью корабл€ и подбита, если -1 то был огонь по пустой клетке
    bool checkalive(const std::array<std::array<int, 10>, 10>& grid, int x, int y, int dx, int dy) {
        // ѕровер€ем границы перед доступом
        if (x + dx < 0 || x + dx >= 10 || y + dy < 0 || y + dy >= 10)
            return false;

        if (grid[x + dx][y + dy] == 1)
            return true;

        if (grid[x + dx][y + dy] == -2) {
            if (x + 2 * dx >= 0 && x + 2 * dx < 10 && y + 2 * dy >= 0 && y + 2 * dy < 10) {
                if (grid[x + 2 * dx][y + 2 * dy] == 1)
                    return true;
                if (grid[x + 2 * dx][y + 2 * dy] == -2) {
                    if (x + 3 * dx >= 0 && x + 3 * dx < 10 && y + 3 * dy >= 0 && y + 3 * dy < 10) {
                        if (grid[x + 3 * dx][y + 3 * dy] == 1)
                            return true;
                    }
                }
            }
        }
        return false;
    }
    bool isvalide(int x, int y) {
        if (x > 9 || x < 0 || y > 9 || y < 0) {
            return false;
        }
        if (y - 1 >= 0) {
            if (grid[x][y - 1] == 1) { return false; }
        }
        if (y + 1 <= 9) {
            if (grid[x][y + 1] == 1) { return false; }
        }
        if (grid[x][y] == 1) { return false; }
        if (x + 1 <= 9) {
            if (grid[x + 1][y] == 1) { return false; }
            if (y + 1 <= 9) {
                if (grid[x + 1][y + 1] == 1) { return false; }
            }
            if (y - 1 >= 0) {
                if (grid[x + 1][y - 1] == 1) { return false; }
            }
        }
        if (x - 1 >= 0) {
            if (grid[x - 1][y] == 1) { return false; }
            if (y + 1 <= 9) {
                if (grid[x - 1][y + 1] == 1) { return false; }
            }
            if (y - 1 >= 0) {
                if (grid[x - 1][y - 1] == 1) { return false; }
            }
        }
        return true;
    }

    void paintcellsfornext(int a) {
        for (std::array<int, 2> i : cellsfornext) {
            grid[i[0]][i[1]] = a;
        }
    }
public:
    Player() = default;
    // ƒобавл€ет корабль(часть корабл€) в указанную клетку 
    std::string givecell(int x, int y) {
        if (x < 0 || x >= 10 || y < 0 || y >= 10) {
            return "wrong cell";
        }

        difgrid.clear();
        std::array<int, 2> step = { x, y };

        if (shipnow.empty()) {
            if (!isvalide(x, y)) {
                return "wrong cell";
            }
            if (countships[1] != 0 || countships[2] != 0 || countships[3] != 0) {
                const std::array<std::array<int, 2>, 4> directions = { { {1, 0}, {-1, 0}, {0, 1}, {0, -1} } };
                for (const auto& dir : directions) {
                    int nx = x + dir[0];
                    int ny = y + dir[1];
                    if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && isvalide(nx, ny)) {
                        if (countships[1] != 0) {
                            cellsfornext.push_back({ nx, ny });
                            difgrid.push_back({ nx, ny, -1 });
                        }
                        else if (countships[2] != 0) {
                            int nnx = nx + dir[0];
                            int nny = ny + dir[1];
                            if (nnx >= 0 && nnx < 10 && nny >= 0 && nny < 10 && isvalide(nnx, nny)) {
                                cellsfornext.push_back({ nx, ny });
                                difgrid.push_back({ nx, ny, -1 });
                            }
                        }
                        else if (countships[3] != 0) {
                            int nnx = nx + 2 * dir[0];
                            int nny = ny + 2 * dir[1];
                            if (nnx >= 0 && nnx < 10 && nny >= 0 && nny < 10 && isvalide(nnx, nny)) {
                                cellsfornext.push_back({ nx, ny });
                                difgrid.push_back({ nx, ny, -1 });
                            }
                        }
                    }
                }
            }
            shipnow.push_back(step);
            grid[x][y] = 1;
            difgrid.push_back({ x, y, 1 });
            paintcellsfornext(-1);
            if (cellsfornext.empty()) {
                if (countships[0] > 0) {
                    countships[0]--;
                    shipnow.clear();
                    return (countships[0] == 0 && countships[1] == 0 &&
                        countships[2] == 0 && countships[3] == 0)
                        ? "switch state" : "new ship";
                }
                return "wrong cell";
            }

            return "continue";
        }
        for (auto i : shipnow) {
            if (i == step) {
                if (countships[shipnow.size() - 1] == 0) {
                    return "error";
                }
                else {
                    for (std::array<int, 2> i : cellsfornext) {
                        difgrid.push_back({ i[0], i[1], 0 });
                    }
                    paintcellsfornext(0);
                    cellsfornext.clear();
                    countships[shipnow.size() - 1]--;
                    shipnow.clear();
                    if (countships[1] == 0 && countships[0] == 0 && countships[2] == 0 && countships[3] == 0) {
                        return "switch state";
                    }
                    else {
                        return "new ship";
                    }
                }
            }
        }
        for (auto i : cellsfornext) {
            if (i == step) {
                bool usl = false;
                paintcellsfornext(0);
                for (std::array<int, 2> i : cellsfornext) {
                    difgrid.push_back({ i[0], i[1], 0 }); 
                }
                shipnow.push_back(step);
                cellsfornext.clear();
                if (shipnow.size() == 2) {
                    if (countships[2] > 0) {
                        int nx = 2 * x - shipnow[0][0];
                        int ny = 2 * y - shipnow[0][1];
                        if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && isvalide(nx, ny)) {
                            cellsfornext.push_back({ nx, ny });
                            difgrid.push_back({ nx, ny, -1 });

                        }
                    }
                    else if (countships[3] > 0) {
                        int nx = 2 * x - shipnow[0][0];
                        int ny = 2 * y - shipnow[0][1];
                        int nnx = 3 * x - 2 * shipnow[0][0];
                        int nny = 3 * y - 2 * shipnow[0][1];
                        if (nnx >= 0 && nnx < 10 && nny >= 0 && nny < 10 && isvalide(nnx, nny)) {
                            cellsfornext.push_back({ nx, ny });
                            difgrid.push_back({ nx, ny, -1 });

                        }
                    }
                }
                else if (shipnow.size() == 3) {
                    if (countships[3] > 0) {
                        const std::array<std::array<int, 2>, 4> directions = { { {1, 0}, {-1, 0}, {0, 1}, {0, -1} } };
                        for (const auto& dir : directions) {
                            int nx = x + dir[0];
                            int ny = y + dir[1];
                            if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && isvalide(nx, ny)) {
                                cellsfornext.push_back({ nx, ny });
                                difgrid.push_back({ nx, ny, -1 });
                            }
                        }
                    }
                }
                if (shipnow.size() == 4) {
                    if (countships[3] == 0) {
                        return "error";
                    }
                    else {
                        grid[x][y] = 1;
                        difgrid.push_back({ x, y, 1 });
                        countships[3]--;
                        shipnow.clear();
                        if (countships[1] == 0 && countships[0] == 0 && countships[2] == 0 && countships[3] == 0) {
                            return "switch state";
                        }
                        else {
                            return "new ship";
                        }
                    }
                }
                else {
                    paintcellsfornext(-1);
                    grid[x][y] = 1;
                    difgrid.push_back({ x, y, 1 });
                }
                if (cellsfornext.size() == 0) {
                    countships[shipnow.size() - 1]--;
                    shipnow.clear();
                    if (countships[1] == 0 && countships[0] == 0 && countships[2] == 0 && countships[3] == 0) {
                        return "switch state";
                    }
                    else {
                        return "new ship";
                    }
                }
                else {
                    return "countinue";
                }
            }
        }
        return "wrong cell";
    }
    void delship(int x, int y) {
        std::array<int, 2> cell = { x, y };
        for (auto i : shipnow) {
            if (i == cell) {
                for (auto j : shipnow) {
                    grid[j[0]][j[1]] = 0;
                }
                shipnow.clear();
                cellsfornext.clear();
            }
        }
    }


    std::string fire(int x, int y) { //огонь по бл€дскому хутору
        if (grid[x][y] == 1) {
            grid[x][y] = -2;
            laststep = { x, y, 1 };
            if (checkalive(grid, x, y, +1, 0) || checkalive(grid, x, y, -1, 0) || checkalive(grid, x, y, 0, +1) || checkalive(grid, x, y, 0, -1)) {
                return "hitted";
            }
            else {
                shipsAlive--;
                return "killed";
            }
        }
        else {
            grid[x][y] = -1;
            laststep = { x, y, -1 };
            return "missed";
        }
    }
    int showcell(int x, int y) const { return grid[x][y]; }

    const std::array<int, 3>& LastStep() const { return laststep; }

    const int getShipsAlive() const { return shipsAlive; }

    std::vector<std::array<int, 3>> showdif() const { return difgrid; }


};
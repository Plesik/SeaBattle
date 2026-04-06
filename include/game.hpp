#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "board.hpp"
#include <array>

class Game {
private:
    enum class GameState {
        Player1Setup,
        Player2Setup,
        Battle1,
        Battle2,
        switchState,
        endState
    };

    sf::RenderWindow& m_window;
    sf::Font& m_font;
    GameState m_currentState = GameState::Player1Setup;

    Player m_player1;
    Player m_player2;
    Board m_board1;
    Board m_board2;

    sf::Text m_titleText;

    std::array<std::array<sf::RectangleShape, 10>, 10> m_grid;

    std::vector<sf::Text> m_rowLabels;
    std::vector<sf::Text> m_colLabels; 

    int m_cellsToPlace = 20;
    int switcher = 0;


    const float cellSize = 40.f;
    const float gridStartX = 250.f;
    const float gridStartY = 200.f;
    

    bool iswin = false;

    const sf::Color MISSEDHIT_COLOR = sf::Color(160, 160, 160);

    const sf::Color HIT_COLOR = sf::Color(100, 100, 100);
    const sf::Color GRID_OUTLINE = sf::Color::Black;
    const sf::Color HOVER_COLOR = sf::Color(200, 200, 200); 
    const sf::Color SELECTED_COLOR = sf::Color(100, 100, 100); 
    const sf::Color BG_COLOR = sf::Color::White;

    void initGrid() {
        for (int x = 0; x < 10; ++x) {
            for (int y = 0; y < 10; ++y) {
                sf::RectangleShape cell;
                cell.setSize({ cellSize, cellSize });
                cell.setPosition(gridStartX + x * cellSize, gridStartY + y * cellSize);
                cell.setFillColor(sf::Color::Transparent);
                cell.setOutlineThickness(1.f);
                cell.setOutlineColor(GRID_OUTLINE); 
                m_grid[x][y] = cell;
            }
        }
        for (int i = 0; i < 10; ++i) {
            sf::Text label;
            label.setFont(m_font);
            label.setString(std::to_string(i + 1));
            label.setCharacterSize(16);
            label.setFillColor(sf::Color::Black);
            label.setPosition(gridStartX - 30.f, gridStartY + i * cellSize + 9.f); 
            m_rowLabels.push_back(label);
        }
        for (int i = 0; i < 10; ++i) {
            sf::Text label;
            label.setFont(m_font);
            label.setString(static_cast<char>('A' + i));
            label.setCharacterSize(16);
            label.setFillColor(sf::Color::Black);
            label.setPosition(gridStartX + i * cellSize + 15.f, gridStartY - 30.f);
            m_colLabels.push_back(label);
        }
    }
    void updateTitlePlace(int switcher) {
        if (m_currentState == GameState::Player1Setup) {
            m_titleText.setString("PLAYER 1 PLACE YOUR SHIPS");
            m_titleText.setFont(m_font);
            m_titleText.setCharacterSize(30);
            m_titleText.setFillColor(sf::Color::Black);
            m_titleText.setPosition((m_window.getSize().x - m_titleText.getLocalBounds().width) / 2, 35.f);
        }
        else if (m_currentState == GameState::Player2Setup) {
            m_titleText.setString("PLAYER 2 PLACE YOUR SHIPS");
        }
        else if (m_currentState == GameState::Battle1) {
            m_titleText.setString("PLAYER 1 MAKE A MOVE");
        }
        else if (m_currentState == GameState::Battle2) {
            m_titleText.setString("PLAYER 2 MAKE A MOVE");
        }
        else if (m_currentState == GameState::switchState) {
            if (switcher == 1) {
                m_titleText.setString("CLICK TO LET PLAYER 1 MAKE MOVE");
            }
            else if (switcher == 2) {
                m_titleText.setString("CLICK TO LET PLAYER 2 MAKE MOVE");
            } 
            else if (switcher == 0) {
                m_titleText.setString("CLICK TO LET PLAYER 2 PLACE SHIPS");
            } 
            else if (switcher == -1) {
                m_titleText.setString("PLAYER 1 WON, CLICK TO SHOW MENU");
            }
            else if (switcher == -2) {
                m_titleText.setString("PLAYER 2 WON, CLICK TO SHOW MENU");
            }
        }
    }

    void handleCellClickPlace(int x, int y) {
        Player& currentPlayer = (m_currentState == GameState::Player1Setup) ? m_player1 : m_player2;
        std::string res = currentPlayer.givecell(x, y);
        for (std::array<int, 3> i : currentPlayer.showdif()) {
            if (i[2] == 0) {
                m_grid[i[0]][i[1]].setFillColor(sf::Color::Transparent);
            }
            else if (i[2] == 1) {
                m_grid[i[0]][i[1]].setFillColor(SELECTED_COLOR);
            }
            else if (i[2] == -1) {
                m_grid[i[0]][i[1]].setFillColor(MISSEDHIT_COLOR);
            }
        }
        if (res == "switch state") {
            if (m_currentState == GameState::Player1Setup) {
                m_board2.getEnemy(m_player1);
            }
            else {
                m_board1.getEnemy(m_player2);
            }
            switchState();
        } 
        else if (res == "wrong cell") {
            m_titleText.setString("wrong cell");
        }
        if (res == "countinue") {
            m_titleText.setString("COUNTINUE PLACE SHIPS");
        }
        if (res == "new ship") {
            m_titleText.setString("Make new ship");
        }
        if (res == "error") {
            m_titleText.setString("error");
        }
    }
    void handleCellClickFire(int x, int y) {
        Board& currentPlayer = (m_currentState == GameState::Battle1) ? m_board1 : m_board2;
        if (m_grid[x][y].getFillColor() != HIT_COLOR && m_grid[x][y].getFillColor() != MISSEDHIT_COLOR) {
            std::string step = currentPlayer.MakeAttack(x, y);
            iswin = currentPlayer.iswin();
            for (std::array<int, 3> list : currentPlayer.showdif()) {
                if (list[2] == -1) {
                    m_grid[list[0]][list[1]].setFillColor(MISSEDHIT_COLOR);
                }
                if (list[2] == -2) {
                    m_grid[list[0]][list[1]].setFillColor(HIT_COLOR);
                }
            }
            if (step == "missed" or iswin) {
                switchState();
            }
        }
    }
    void handleMouseClickSwitch() {
        switchState();
    }

    void switchState() {
        int nextplayer = 0;
        if (m_currentState == GameState::Player1Setup) {
            m_currentState = GameState::switchState;
            m_cellsToPlace = 20;
            switcher = 0;
            updateTitlePlace(switcher);
        }
        else if (m_currentState == GameState::switchState) {
            if (switcher == 1) {
                resetGrid();
                m_currentState = GameState::Battle1;
                updateTitlePlace(switcher);
                if (!iswin) {
                    for (int i = 0; i < 10; i++) {
                        for (int j = 0; j < 10; j++) {
                            if (m_board1.showcell(i, j) == -1) {
                                m_grid[i][j].setFillColor(MISSEDHIT_COLOR);
                            }
                            if (m_board1.showcell(i, j) == -2) {
                                m_grid[i][j].setFillColor(HIT_COLOR);
                            }
                        }
                    }
                }
            }
            if (switcher == 0) {
                resetGrid();
                m_currentState = GameState::Player2Setup;
                updateTitlePlace(switcher);
            }
            if (switcher == 2) {
                resetGrid();
                m_currentState = GameState::Battle2;
                updateTitlePlace(switcher);
                if (!iswin) {
                    for (int i = 0; i < 10; i++) {
                        for (int j = 0; j < 10; j++) {
                            if (m_board2.showcell(i, j) == -1) {
                                m_grid[i][j].setFillColor(MISSEDHIT_COLOR);
                            }
                            if (m_board2.showcell(i, j) == -2) {
                                m_grid[i][j].setFillColor(HIT_COLOR);
                            }
                        }
                    }
                }
            }
            if (switcher == -1 || switcher == -2) {
                m_currentState = GameState::endState;
            }
        }
        else if (m_currentState == GameState::Player2Setup) {
            m_currentState = GameState::switchState;
            switcher = 1;
            updateTitlePlace(switcher);
        }
        else if (m_currentState == GameState::Battle1) {
            if (iswin) {
                switcher = -1;
            }
            else {
                switcher = 2;
            }
            m_currentState = GameState::switchState;
            updateTitlePlace(switcher);
        }
        else if (m_currentState == GameState::Battle2) {
            if (iswin) {
                switcher = -2;
            }
            else {
                switcher = 1;
            }
            m_currentState = GameState::switchState;
            updateTitlePlace(switcher);
        }
    }

    void resetGrid() {
        for (auto& row : m_grid) {
            for (auto& cell : row) {
                cell.setFillColor(sf::Color::Transparent);
            }
        }
    }

    void handleMouseClick(const sf::Vector2i& mousePixel) {
        int x = (mousePixel.x - static_cast<int>(gridStartX)) / static_cast<int>(cellSize);
        int y = (mousePixel.y - static_cast<int>(gridStartY)) / static_cast<int>(cellSize);
        if (m_currentState == GameState::Battle1 || m_currentState == GameState::Battle2) {
            if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                handleCellClickFire(x, y);
            }
        } else if (m_currentState == GameState::Player1Setup || m_currentState == GameState::Player2Setup) {
            if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                handleCellClickPlace(x, y);
            }
        } else if (m_currentState == GameState::switchState || m_currentState == GameState::endState) {
            handleMouseClickSwitch();
        }
    }
    void updateHoveredCell(const sf::Vector2i& mousePixel) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (m_grid[i][j].getFillColor() != SELECTED_COLOR && m_grid[i][j].getFillColor() != MISSEDHIT_COLOR && m_grid[i][j].getFillColor() != HIT_COLOR) {
                    m_grid[i][j].setFillColor(sf::Color::Transparent);
                }
            }
        }

        int x = (mousePixel.x - static_cast<int>(gridStartX)) / static_cast<int>(cellSize);
        int y = (mousePixel.y - static_cast<int>(gridStartY)) / static_cast<int>(cellSize);

        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            if (m_grid[x][y].getFillColor() == sf::Color::Transparent) {
                m_grid[x][y].setFillColor(HOVER_COLOR);
            }
        }
    }

public:
    Game(sf::RenderWindow& window, sf::Font& font)
        : m_window(window), m_font(font), m_board1(m_player2), m_board2(m_player1) {
        initGrid();
        updateTitlePlace(switcher);
    }


    void run() {
        while (m_window.isOpen()) {
            sf::Event event;
            while (m_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    m_window.close();
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(sf::Mouse::getPosition(m_window));
                }
            }

            updateHoveredCell(sf::Mouse::getPosition(m_window));

            m_window.clear(BG_COLOR);
            m_window.draw(m_titleText);
            for (const auto& row : m_grid) {
                for (const auto& cell : row) {
                    m_window.draw(cell);
                }
            }
            for (const auto& label : m_rowLabels) m_window.draw(label);
            for (const auto& label : m_colLabels) m_window.draw(label);

            m_window.display();
            if (m_currentState == GameState::endState) {
                break;
            }
        }
    }
};
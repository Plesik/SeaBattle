#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Rules {
public:
    Rules(sf::RenderWindow& window, sf::Font& font)
        : m_window(window), m_font(font) {
        setupText();
    }

    bool run() {
        while (m_window.isOpen()) {
            if (!handleEvents()) break;
            render();
        }
        return true;
    }

private:
    sf::RenderWindow& m_window;
    sf::Font& m_font;
    std::vector<sf::Text> m_rulesText;
    sf::Text m_backText;

    void setupText() {
        // Game rules in English
        std::vector<std::string> rules = {
            "Sea Battle Game Rules:",
            "1. Two players compete.",
            "2. Each player places 10 ships:",
            "   - 1 Battleship (4 cells)",
            "   - 2 Cruisers (3 cells)",
            "   - 3 Destroyers (2 cells)",
            "   - 4 Submarines (1 cell)",
            "3. Players take turns firing shots.",
            "4. The first to sink all opponent's ships wins.",
            "",
            "Press ESC to return to menu"
        };

        for (size_t i = 0; i < rules.size(); ++i) {
            sf::Text text;
            text.setFont(m_font);
            text.setString(rules[i]);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(100.f, 100.f + i * 30.f);
            m_rulesText.push_back(text);
        }

        // Back button
        m_backText.setFont(m_font);
        m_backText.setString("Back (ESC)");
        m_backText.setCharacterSize(24);
        m_backText.setFillColor(sf::Color::Green);
        m_backText.setPosition(100.f, 500.f);
    }

    bool handleEvents() {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                return false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return false; // просто выходим
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (m_backText.getGlobalBounds().contains(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y))) {
                    return false;
                }
            }
        }
        return true;
    }

    void render() {
        m_window.clear(sf::Color(30, 30, 60)); // Dark blue background

        // Draw all rules text
        for (const auto& text : m_rulesText) {
            m_window.draw(text);
        }

        // Draw back button
        m_window.draw(m_backText);

        m_window.display();
    }
};
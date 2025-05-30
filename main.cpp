#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "rules.hpp"
#include "game.hpp"
#include <iostream>
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Sea Battle");
    sf::Font font;
    if (!font.loadFromFile("ArialRegular.ttf")) return 1;


    Button playBtn("Play", font, sf::Vector2f(200, 50), 0, sf::Color::Black, sf::Color::White);
    Button rulesBtn("Rules", font, sf::Vector2f(200, 50), 0, sf::Color::Black, sf::Color::White);
    Button exitBtn("Exit", font, sf::Vector2f(200, 50), 0, sf::Color::Black, sf::Color::White);
    // Позиционирование кнопок
    playBtn.setPosition(sf::Vector2f(500, 300));
    rulesBtn.setPosition(sf::Vector2f(500, 400));
    exitBtn.setPosition(sf::Vector2f(500, 500));

    // Заголовок игры
    sf::Text title("Sea Battle", font, 70);
    title.setFillColor(sf::Color::Black);
    title.setPosition(450, 100);

    // Главный цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (playBtn.isMouseOver(window)) {
                        Game game(window, font); // Создаём игру
                        game.run();       // Запускаем игровой цикл
                        // После выхода из игры управление вернётся сюда
                    }
                    else if (rulesBtn.isMouseOver(window)) {
                        Rules rules(window, font);
                        rules.run(); // Возврат в меню без закрытия окна
                    }
                        // Здесь будет переход к правилам
                    
                    else if (exitBtn.isMouseOver(window)) {
                        window.close();
                    }
                }
            }
        }

        // Обновление кнопок
        playBtn.update(window);
        rulesBtn.update(window);
        exitBtn.update(window);

        // Отрисовка
        window.clear(sf::Color::White);
        window.draw(title);
        playBtn.draw(window);
        rulesBtn.draw(window);
        exitBtn.draw(window);
        window.display();
    }

    return 0;
}
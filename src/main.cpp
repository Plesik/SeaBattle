#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "rules.hpp"
#include "game.hpp"
#include <iostream>
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Sea Battle");
    sf::Font font;
    if (!font.loadFromFile("assets/ArialRegular.ttf")) return 1;


    Button playBtn("Play", font, sf::Vector2f(200.f, 50.f), 15.f, sf::Color::Black, sf::Color::White);
    playBtn.waitUntilReady();

    Button rulesBtn("Rules", font, sf::Vector2f(200.f, 50.f), 15.f, sf::Color::Black, sf::Color::White);
    Button exitBtn("Exit", font, sf::Vector2f(200.f, 50.f), 15.f, sf::Color::Black, sf::Color::White);
    playBtn.setPosition(sf::Vector2f(500, 300));
    rulesBtn.setPosition(sf::Vector2f(500, 400));
    exitBtn.setPosition(sf::Vector2f(500, 500));

    sf::Text title("Sea Battle", font, 70);
    title.setFillColor(sf::Color::Black);
    title.setPosition(450, 100);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (playBtn.isMouseOver(window)) {
                        Game game(window, font);
                        game.run();
                    }
                    else if (rulesBtn.isMouseOver(window)) {
                        Rules rules(window, font);
                        rules.run();
                    }             
                    else if (exitBtn.isMouseOver(window)) {
                        window.close();
                    }
                }
            }
        }
        playBtn.update(window);
        rulesBtn.update(window);
        exitBtn.update(window);
        window.clear(sf::Color::White);
        window.draw(title);
        playBtn.draw(window);
        rulesBtn.draw(window);
        exitBtn.draw(window);
        window.display();
    }

    return 0;
}
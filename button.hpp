#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Button {
private:
    sf::ConvexShape shape;
    sf::Text text;
    sf::Color defaultColor;
    sf::Color hoverColor;
    float cornerRadius;
    sf::Vector2f size;
    unsigned pointCount = 30;

    void updateRoundedShape() {
        shape.setPointCount(pointCount * 4);
        const float pi = 3.141592654f;
        unsigned point = 0;

        // Левый верхний угол
        for (unsigned i = 0; i < pointCount; ++i) {
            float angle = pi * 0.5f + (pi * i) / (pointCount * 2);
            shape.setPoint(point++, {
                cornerRadius * (1 + std::cos(angle)),
                cornerRadius * (1 + std::sin(angle))
                });
        }

        // Правый верхний угол
        for (unsigned i = 0; i < pointCount; ++i) {
            float angle = (pi * i) / (pointCount * 2);
            shape.setPoint(point++, {
                size.x - cornerRadius * std::cos(angle),
                cornerRadius * (1 - std::sin(angle))
                });
        }

        // Правый нижний угол
        for (unsigned i = 0; i < pointCount; ++i) {
            float angle = pi * 1.5f + (pi * i) / (pointCount * 2);
            shape.setPoint(point++, {
                size.x - cornerRadius * std::cos(angle),
                size.y - cornerRadius * std::sin(angle)
                });
        }

        // Левый нижний угол
        for (unsigned i = 0; i < pointCount; ++i) {
            float angle = pi * 1.0f + (pi * i) / (pointCount * 2);
            shape.setPoint(point++, {
                cornerRadius * (1 - std::cos(angle)),
                size.y - cornerRadius * (1 - std::sin(angle))
                });
        }
    }

public:
    Button(const std::string& btnText, sf::Font& font, sf::Vector2f btnSize,
        float radius, sf::Color bgColor, sf::Color textColor)
        : cornerRadius(radius), size(btnSize) {

        // Проверка на минимальный размер
        if (size.x < 2 * cornerRadius) size.x = 2 * cornerRadius;
        if (size.y < 2 * cornerRadius) size.y = 2 * cornerRadius;

        text.setString(btnText);
        text.setFont(font);
        text.setFillColor(textColor);
        text.setCharacterSize(24);

        shape.setFillColor(bgColor);
        defaultColor = bgColor;
        hoverColor = sf::Color(
            std::min(bgColor.r + 50, 255),
            std::min(bgColor.g + 50, 255),
            std::min(bgColor.b + 50, 255)
        );

        updateRoundedShape();
    }

    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        float xPos = pos.x + (size.x - text.getLocalBounds().width) / 2;
        float yPos = pos.y + (size.y - text.getLocalBounds().height) / 2 - 5;
        text.setPosition(xPos, yPos);
    }

    bool isMouseOver(sf::RenderWindow& window) const {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return shape.getGlobalBounds().contains(static_cast<float>(mousePos.x),
            static_cast<float>(mousePos.y));
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
        window.draw(text);
    }

    void update(sf::RenderWindow& window) {
        if (isMouseOver(window)) {
            shape.setFillColor(hoverColor);
        }
        else {
            shape.setFillColor(defaultColor);
        }
    }
};
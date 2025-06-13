#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <mutex>
#include <future>

class Button {
private:
    sf::ConvexShape shape;
    sf::Text text;
    sf::Color defaultColor, hoverColor;
    float cornerRadius;
    sf::Vector2f size;
    unsigned pointCount = 30;
    std::mutex mutex;

    // Async task future
    std::future<void> shapeFuture;

    void computeShapePoints() {
        sf::ConvexShape temp;
        temp.setPointCount(pointCount * 4);

        const float pi = 3.141592654f;
        unsigned point = 0;

        cornerRadius = std::min(cornerRadius, std::min(size.x, size.y) / 2.f);
        sf::Vector2f topLeft(cornerRadius, cornerRadius);
        sf::Vector2f topRight(size.x - cornerRadius, cornerRadius);
        sf::Vector2f bottomRight(size.x - cornerRadius, size.y - cornerRadius);
        sf::Vector2f bottomLeft(cornerRadius, size.y - cornerRadius);

        auto addArc = [&](sf::Vector2f center, float startAngle) {
            for (unsigned i = 0; i < pointCount; ++i) {
                float angle = startAngle + (pi / 2.f) * (float(i) / (pointCount - 1));
                temp.setPoint(point++, {
                    center.x + cornerRadius * std::cos(angle),
                    center.y + cornerRadius * std::sin(angle)
                    });
            }
        };

        addArc(topLeft, pi);
        addArc(topRight, 1.5f * pi);
        addArc(bottomRight, 0.f);
        addArc(bottomLeft, 0.5f * pi);

        std::lock_guard<std::mutex> lock(mutex);
        shape = temp;
        shape.setFillColor(defaultColor);
    }

public:
    Button(const std::string& btnText, sf::Font& font, sf::Vector2f btnSize,
        float radius, sf::Color bgColor, sf::Color textColor)
        : cornerRadius(radius), size(btnSize), defaultColor(bgColor) {

        text.setString(btnText);
        text.setFont(font);
        text.setFillColor(textColor);
        text.setCharacterSize(24);

        hoverColor = sf::Color(
            std::min(bgColor.r + 50, 255),
            std::min(bgColor.g + 50, 255),
            std::min(bgColor.b + 50, 255)
        );

        // Строим форму в отдельном потоке
        shapeFuture = std::async(std::launch::async, [this]() {
            computeShapePoints();
            });
    }

    void setPosition(sf::Vector2f pos) {
        std::lock_guard<std::mutex> lock(mutex);
        shape.setPosition(pos);
        float xPos = pos.x + (size.x - text.getLocalBounds().width) / 2.f;
        float yPos = pos.y + (size.y - text.getLocalBounds().height) / 2.f - 5.f;
        text.setPosition(xPos, yPos);
    }
    bool isMouseOver(sf::RenderWindow& window) const {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return shape.getGlobalBounds().contains(
            static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)
        );
    }
    void waitUntilReady() {
        if (shapeFuture.valid()) shapeFuture.get();
    }



    void draw(sf::RenderWindow& window) {
        std::lock_guard<std::mutex> lock(mutex);
        window.draw(shape);
        window.draw(text);
    }

    void update(sf::RenderWindow& window) {
        std::lock_guard<std::mutex> lock(mutex);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool isOver = shape.getGlobalBounds().contains(
            static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        shape.setFillColor(isOver ? hoverColor : defaultColor);
    }
};
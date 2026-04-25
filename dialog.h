#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include <string>
#include <functional>
#include "screenConfig.h"

using namespace sf;

class Dialog {
public:
    enum class Choice {
        None,
        Stop,
        Continue
    };

private:
    RectangleShape background;
    RectangleShape buttonStop;
    RectangleShape buttonContinue;

    Text messageText;
    Text stopText;
    Text continueText;

    Font& font;

    bool active;
    Choice currentChoice;

    // Колбэки для кнопок
    std::function<void()> onStopCallback;
    std::function<void()> onContinueCallback;

public:
    Dialog() : font(ResourceManager::getFont(0)), 
        active(false),
        currentChoice(Choice::None),
        messageText(font),
        stopText(font),
        continueText(font) {
        // Фон диалога
        background.setSize(ScreenConfig::size(800, 300));
        background.setFillColor(Color(50, 50, 50, 230));
        background.setOutlineColor(Color::White);
        background.setOutlineThickness(3);
        background.setOrigin(ScreenConfig::size(400, 150));
        background.setPosition(ScreenConfig::pos(960, 540));

        // Кнопка "Остановиться"
        buttonStop.setSize(ScreenConfig::size(250, 60));
        buttonStop.setFillColor(Color(150, 50, 50));
        buttonStop.setOutlineColor(Color::White);
        buttonStop.setOutlineThickness(2);
        buttonStop.setOrigin(ScreenConfig::size(125, 30));
        buttonStop.setPosition(ScreenConfig::pos(760, 620));

        // Кнопка "Продолжить"
        buttonContinue.setSize(ScreenConfig::size(250, 60));
        buttonContinue.setFillColor(Color(50, 100, 50));
        buttonContinue.setOutlineColor(Color::White);
        buttonContinue.setOutlineThickness(2);
        buttonContinue.setOrigin(ScreenConfig::size(125, 30));
        buttonContinue.setPosition(ScreenConfig::pos(1160, 620));

        // Текст сообщения
        messageText.setFont(font);
        messageText.setString("Boss Vegan says:\n\n\"Stop stealing eggs from chickens!\"");
        messageText.setCharacterSize(static_cast<unsigned int>(32 * ScreenConfig::scaleY));
        messageText.setFillColor(Color::White);
        messageText.setOutlineColor(Color::Black);
        messageText.setOutlineThickness(1);
        messageText.setPosition(ScreenConfig::pos(960, 480));

        // Текст кнопок
        stopText.setFont(font);
        stopText.setString("STOP");
        stopText.setCharacterSize(28);
        stopText.setFillColor(Color::White);
        stopText.setOutlineColor(Color::Black);
        stopText.setOutlineThickness(1);

        continueText.setFont(font);
        continueText.setString("CONTINUE");
        continueText.setCharacterSize(28);
        continueText.setFillColor(Color::White);
        continueText.setOutlineColor(Color::Black);
        continueText.setOutlineThickness(1);

        centerTextOrigins();
    }

    void centerTextOrigins() {
        FloatRect msgBounds = messageText.getLocalBounds();
        messageText.setOrigin(Vector2f(msgBounds.size.x / 2, msgBounds.size.y / 2));

        FloatRect stopBounds = stopText.getLocalBounds();
        stopText.setOrigin(Vector2f(stopBounds.size.x / 2, stopBounds.size.y / 2));
        stopText.setPosition(ScreenConfig::pos(760, 610));

        FloatRect contBounds = continueText.getLocalBounds();
        continueText.setOrigin(Vector2f(contBounds.size.x / 2, contBounds.size.y / 2));
        continueText.setPosition(ScreenConfig::pos(1160, 610));
    }

    void show() {
        active = true;
        currentChoice = Choice::None;
    }

    void hide() {
        active = false;
    }

    bool isActive() const {
        return active;
    }

    Choice getChoice() const {
        return currentChoice;
    }

    void setCallbacks(std::function<void()> onStop, std::function<void()> onContinue) {
        onStopCallback = onStop;
        onContinueCallback = onContinue;
    }

    void handleInput(const RenderWindow& window) {
        if (!active) return;

        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f worldPos = window.mapPixelToCoords(mousePos);

        // Подсветка кнопок при наведении
        bool hoverStop = buttonStop.getGlobalBounds().contains(worldPos);
        bool hoverContinue = buttonContinue.getGlobalBounds().contains(worldPos);

        buttonStop.setFillColor(hoverStop ? Color(200, 70, 70) : Color(150, 50, 50));
        buttonContinue.setFillColor(hoverContinue ? Color(70, 150, 70) : Color(50, 100, 50));

        // Обработка кликов
        static bool mouseWasPressed = false;
        bool mouseIsPressed = Mouse::isButtonPressed(Mouse::Button::Left);

        if (mouseIsPressed && !mouseWasPressed) {
            if (hoverStop) {
                currentChoice = Choice::Stop;
                if (onStopCallback) onStopCallback();
                hide();
            }
            else if (hoverContinue) {
                currentChoice = Choice::Continue;
                if (onContinueCallback) onContinueCallback();
                hide();
            }
        }

        mouseWasPressed = mouseIsPressed;
    }

    void draw(RenderWindow& window) {
        if (!active) return;

        window.draw(background);
        window.draw(buttonStop);
        window.draw(buttonContinue);
        window.draw(messageText);
        window.draw(stopText);
        window.draw(continueText);
    }
};
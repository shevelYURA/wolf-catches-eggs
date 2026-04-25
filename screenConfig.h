#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class ScreenConfig {
public:
    static Vector2f scale;
    static float scaleX;
    static float scaleY;

    static void init(unsigned int width, unsigned int height) {
        scaleX = width / 1920.0f;
        scaleY = height / 1080.0f;
        scale = Vector2f(scaleX, scaleY);
    }

    static Vector2f pos(float x, float y) {
        return Vector2f(x * scaleX, y * scaleY);
    }

    static Vector2f size(float w, float h) {
        return Vector2f(w * scaleX, h * scaleY);
    }
};
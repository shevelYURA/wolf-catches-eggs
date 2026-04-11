#pragma once
#include "FallingObject.h"
#include "resource.h"

class Egg : public FallingObject {
private:
    bool isGolden;  // золотое яйцо или нет

public:
    Egg() : FallingObject("image/eggsTexture.png", Vector2f(40, 35)), isGolden(false) {}
    
    void setGolden(bool golden) {
        isGolden = golden;
        if (isGolden) {
            setColor(sf::Color(255, 215, 0));  // Золотой цвет
        } else {
            setColor(sf::Color::White);        // Белый цвет
        }
    }
    
    bool getGolden() const {
        return isGolden;
    }
};

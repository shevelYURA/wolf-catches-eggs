#pragma once
#include "FallingObject.h"
#include "resource.h"

class Egg : public FallingObject {
private:
    bool isGolden;

public:
    Egg() : FallingObject(IDB_PNG5, Vector2f(40, 35)), isGolden(false) {}
    
    void setGolden(bool golden) {
        isGolden = golden;
        if (isGolden) {
            setColor(sf::Color(255, 215, 0)); // Золотой цвет
        } else {
            setColor(sf::Color::White); // Белый цвет
        }
    }
    
    bool getGolden() const {
        return isGolden;
    }
    
    // Методы для режима яйцепада
    void enableRainMode() { enableEggRainMode(); }
    void disableRainMode() { disableEggRainMode(); }
    bool isInRainMode() const { return isEggRainMode(); }
    
    // Принудительное падение в указанной позиции
    void forceRainFall(float x, float y) {
        forceFall(Vector2f(x, y));
    }
};

#pragma once
#include "FallingObject.h"
#include "resource.h"
#include "screenConfig.h"

enum class PowerUpType {
    DoublePoints,
    EggRain,
    BoxingGlove,
    Burger,    // Восстанавливает 25 HP
    Potion     // Восстанавливает 40 HP
};

class PowerUp : public FallingObject {
private:
    PowerUpType type;

public:
    PowerUp(PowerUpType t = PowerUpType::EggRain) 
        : FallingObject(
            (t == PowerUpType::BoxingGlove) ? IDB_BOXING_GLOVE :
            (t == PowerUpType::Burger) ? IDB_BURGER :
            (t == PowerUpType::Potion) ? IDB_POTION : IDB_POWERUP,
            ScreenConfig::size(40, 40)
          ), type(t) {}

    PowerUpType getType() const { return type; }
    void setType(PowerUpType t) { type = t; }
};

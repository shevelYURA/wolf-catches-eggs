#pragma once
#include "FallingObject.h"
#include "resource.h"

enum class PowerUpType {
    DoublePoints,
    EggRain,
    BoxingGlove,
    Burger,    // ← ВОССТАНАВЛИВАЕТ 25 HP (ЧАСТО)
    Potion     // ← ВОССТАНАВЛИВАЕТ 40 HP (РЕДКО)
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
            Vector2f(40, 40)
          ), type(t) {}

    PowerUpType getType() const { return type; }
    void setType(PowerUpType t) { type = t; }
};

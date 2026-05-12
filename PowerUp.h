#pragma once
#include "FallingObject.h"
#include "resource.h"

enum class PowerUpType {
    DoublePoints,
    EggRain
};

class PowerUp : public FallingObject {
private:
    PowerUpType type;

public:
    PowerUp(PowerUpType t = PowerUpType::EggRain) 
        : FallingObject(IDB_POWERUP, Vector2f(40, 40)), type(t) {}

    PowerUpType getType() const { return type; }
    void setType(PowerUpType t) { type = t; }
};

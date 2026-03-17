#pragma once
#include "FallingObject.h"

class Bomb : public FallingObject {
public:
    Bomb() : FallingObject("image/bombTexture.png", Vector2f(40, 35)) {}
};
#pragma once
#include "FallingObject.h"

class Egg : public FallingObject {
public:
    Egg() : FallingObject("image/eggsTexture.png", Vector2f(40, 35)) {}
};
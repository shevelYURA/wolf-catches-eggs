#pragma once
#include "FallingObject.h"
#include "resource.h"

class Bomb : public FallingObject {
public:
    Bomb() : FallingObject(IDB_PNG3, Vector2f(40, 35)) {}
};
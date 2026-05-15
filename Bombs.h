#pragma once
#include "FallingObject.h"
#include "resource.h"

class Bomb : public FallingObject {
public:
    Bomb() : FallingObject(IDB_BOMB, Vector2f(30, 30)) {}
};

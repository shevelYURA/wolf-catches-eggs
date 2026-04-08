#pragma once
#include "FallingObject.h"
#include "resource.h"

class Egg : public FallingObject {
public:
    Egg() : FallingObject(IDB_PNG5, Vector2f(40, 35)) {}
};
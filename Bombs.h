#pragma once
#include "FallingObject.h"
#include "resource.h"
#include "screenConfig.h"

class Bomb : public FallingObject {
public:
    Bomb() : FallingObject(IDB_BOMB, ScreenConfig::size(50, 50)) {}
};

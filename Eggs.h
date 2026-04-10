#pragma once
#include "FallingObject.h"

class Egg : public FallingObject {
private:
    bool isGolden;  //золотое яйцо или нет

public:
    Egg() : FallingObject("image/eggsTexture.png", Vector2f(40, 35)), isGolden(false) {}
    
    void setGolden(bool golden) {
        isGolden = golden;
    }
    
    bool getGolden() const {
        return isGolden;
    }
};

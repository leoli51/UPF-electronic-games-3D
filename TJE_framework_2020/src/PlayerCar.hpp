//
//  PlayerController.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 13/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef PlayerController_hpp
#define PlayerController_hpp

#include "Vehicle.hpp"
#include "q3.h"

class PlayerCar : public Vehicle {
public:
    float turn_strength = 10;
    float acceleration = 50;
    
    PlayerCar(q3Scene* scene);
    
    void update(float dt);
};

#endif /* PlayerController_hpp */

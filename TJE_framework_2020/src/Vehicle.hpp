//
//  Vehicle.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 11/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef Vehicle_hpp
#define Vehicle_hpp

#include "framework.h"
#include "entity.h"
#include "q3.h"

class Vehicle : public Entity {
public:
    
    q3Body* body;
    
    Vehicle();
    ~Vehicle();
    
    void setBody(q3Body* body);
    
    void accelerate(float amount);
    void turn(float amount);
    void handbrake();
    
    void update(float dt);
};

class VehicleFactory {
public:
    static Vehicle* createVehicle(q3Scene* scene);
};

#endif /* Vehicle_hpp */

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
    virtual ~Vehicle();
    
    void setBody(q3Body* body);
    
    // positive accelerate negative decelerate
    void accelerate(float amount);
    // positive turn right negative turn left
    void turn(float amount);
    //handbrake
    void handbrake();
    
    q3Vec3 getLateralVelocity();
    
    void update(float dt);
    
    void setPosition(float x, float y, float z);
    
protected:
    void updateFriction(float dt);
    
};

class VehicleFactory {
public:
    static Vehicle* createVehicle(q3Scene* scene);
};

#endif /* Vehicle_hpp */

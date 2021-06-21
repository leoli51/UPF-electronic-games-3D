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
    float turn_strength = 10;
    float acceleration_strength = 40;
    float skidding_velocity = .6f;
    float max_forward_speed = 40;
    float max_backwards_speed = -5;
    
    q3Body* body;
    
    Vehicle(std::string model_name, q3Scene *scene);
    virtual ~Vehicle();
    
    //void setBody(q3Body* body);
    
    // positive accelerate negative decelerate
    void accelerate(float amount);
    // positive turn right negative turn left
    void turn(float amount);
    //handbrake
    void handbrake();
    
    q3Vec3 getLateralVelocity();
    q3Vec3 getForwardVelocity();
    
    float getSpeed();
    
    void update(float dt);
    
    void setPosition(float x, float y, float z);
    
protected:
    void updateFriction(float dt);
    
};

#endif /* Vehicle_hpp */

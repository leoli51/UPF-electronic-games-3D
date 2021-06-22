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
#include "bodyentity.hpp"
#include "q3.h"

class Vehicle : public BodyEntity {
public:
    float turn_strength = 10;
    float acceleration_strength = 40;
    float skidding_velocity = 1;
    float handbrake_strength = 0.6f;
    float handbrake_skidding_scale = 0.5f;
    bool braking = false;
    float max_forward_speed = 40;
    float max_backwards_speed = -5;
    
    Vehicle(std::string model_name, q3BodyDef def, q3Scene *scene);
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
    
    
protected:
    void updateFriction(float dt);
    
};

#endif /* Vehicle_hpp */

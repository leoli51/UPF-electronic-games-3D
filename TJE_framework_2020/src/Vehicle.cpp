//
//  Vehicle.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 11/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "Vehicle.hpp"


Vehicle::Vehicle(std::string model_name, q3BodyDef def, q3Scene *scene) : BodyEntity(def, scene) {
    BodyEntity::setMesh(model_name);
};

void Vehicle::accelerate(float amount){
    if (getSpeed() > max_forward_speed && amount > 0)
        return;
    if (getSpeed() < max_backwards_speed && amount < 0)
        return;
    body->ApplyLinearForce(body->GetWorldVector(q3Vec3(0,0,amount * acceleration_strength)));
};

void Vehicle::turn(float amount){
    body->ApplyTorque(q3Vec3(0,amount * turn_strength,0));
};

void Vehicle::handbrake(){
    q3Vec3 stop_force = body->GetLinearVelocity();
    stop_force *= -handbrake_strength;
    
    body->ApplyLinearForce(stop_force);
    braking = true;
};

q3Vec3 Vehicle::getLateralVelocity(){
    q3Vec3 side_normal = body->GetWorldVector(q3Vec3(1,0,0));
    q3Vec3 lateral_velocity = side_normal * q3Dot(side_normal, body->GetLinearVelocity());
    return lateral_velocity;
};

q3Vec3 Vehicle::getForwardVelocity(){
    q3Vec3 front_normal = body->GetWorldVector(q3Vec3(0,0,1));
    q3Vec3 front_velocity = front_normal * q3Dot(front_normal, body->GetLinearVelocity());
    return front_velocity;
};

float Vehicle::getSpeed(){
    return q3Dot(getForwardVelocity(), body->GetWorldVector(q3Vec3(0,0,1)));
};

void Vehicle::updateFriction(float dt){
    // kill lateral velocity
    q3Vec3 impulse = -getLateralVelocity() * body->GetMass();
    //std::cout<<q3Length(impulse)<<std::endl;
    if (q3Length(impulse) >= skidding_velocity){
        impulse /= q3Length(impulse);
        impulse *= skidding_velocity;
    }
    if (braking)
        impulse *= handbrake_skidding_scale;
    braking = false;
    body->ApplyLinearImpulse(impulse);
    
    // kill angular velocity note: should be getInertia instead of getMass
    body->ApplyAngularImpulse(-body->GetAngularVelocity() * /*body->GetMass() */ 0.1f);
};

void Vehicle::update(float elapsed_time){
    BodyEntity::update(elapsed_time);
    updateFriction(elapsed_time);
};


Vehicle::~Vehicle(){
    
};


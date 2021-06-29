//
//  Vehicle.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 11/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "Vehicle.hpp"
#include "q3Factory.h"

VehicleRayCallback::VehicleRayCallback(Vehicle *vehicle){
    this->vehicle = vehicle;
    this->ignore = vehicle->box;
};

bool VehicleRayCallback::ReportShape(q3Box *box){
    if (box == ignore) return true;
    const int eStatic = 0x020;
    //int eDynamic = 0x040;
    if (box->body->GetFlags() & eStatic && box->body->GetUserData() != NULL){
        vehicle->on_ground = true;
        return false;
    }
    return true;
};

Vehicle::Vehicle(std::string model_name, q3BodyDef def, q3Scene *scene) : BodyEntity(createBodydef(eDynamicBody, (void*) this), scene) {
    BodyEntity::setMesh(model_name);
};

void Vehicle::accelerate(float amount){
    if (!on_ground || exploded) return;
    
    if (getSpeed() > max_forward_speed && amount > 0)
        return;
    if (getSpeed() < max_backwards_speed && amount < 0)
        return;
    body->ApplyLinearForce(body->GetWorldVector(q3Vec3(0,0,amount * acceleration_strength)));
};

void Vehicle::turn(float amount){
    if (exploded) return;
    
    float scale = on_ground ? 1 : .2f;
    body->ApplyTorque(q3Vec3(0,amount * turn_strength * scale,0));
};

void Vehicle::handbrake(){
    if (!on_ground || exploded) return;
    
    q3Vec3 stop_force = body->GetLinearVelocity();
    stop_force *= -handbrake_strength;
    
    body->ApplyLinearForce(stop_force);
    braking = true;
};

void Vehicle::explode(){
    default_kd = Vector4(0,0,0,1);
    use_default_kd = true;
    
    exploded = true;
    body->ApplyLinearImpulse(q3Vec3(0,10,0));
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
    if (exploded)
        time_since_explosion += elapsed_time;
    if (on_ground)
        updateFriction(elapsed_time);
    
    //Vector3 ground_normal(0,1,0);
    //Vector3 top = transform.topVector();
    //float dotp = dot(ground_normal, top);
    //if (dotp <= 0.1 ) // onground
    //    body->ApplyAngularImpulse(q3Vec3(0,0,.1));
    
    on_ground = false;
    
    q3RaycastData raycast;
    Vector3 down_dir = -1 * transform.topVector().normalize();
    raycast.Set(body->GetTransform().position, q3Vec3(down_dir.x, down_dir.y, down_dir.z), mesh->box.halfsize.y + .1f);
    
    VehicleRayCallback callback(this);
    scene->RayCast(&callback, raycast);
};


Vehicle::~Vehicle(){
    
};


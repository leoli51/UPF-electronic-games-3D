//
//  Vehicle.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 11/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "Vehicle.hpp"


Vehicle::Vehicle(){
    
};

void Vehicle::setBody(q3Body *body){
    this->body = body;
};

void Vehicle::accelerate(float amount){
    body->ApplyLinearForce(body->GetWorldVector(q3Vec3(0,0,amount)));
};

void Vehicle::turn(float amount){
    body->ApplyTorque(q3Vec3(0,amount,0));
};

void Vehicle::handbrake(){
    
};

q3Vec3 Vehicle::getLateralVelocity(){
    q3Vec3 side_normal = body->GetWorldVector(q3Vec3(1,0,0));
    q3Vec3 lateral_velocity = side_normal * q3Dot(side_normal, body->GetLinearVelocity());
    return lateral_velocity;
};

void Vehicle::updateFriction(float dt){
    // kill lateral velocity
    q3Vec3 impulse = -getLateralVelocity() * body->GetMass();
    body->ApplyLinearImpulse(impulse);
    
    // kill angular velocity note: should be getInertia instead of getMass
    body->ApplyAngularImpulse(-body->GetAngularVelocity() * /*body->GetMass() */ 0.1f);
};

void Vehicle::update(float elapsed_time){
    updateFriction(elapsed_time);
    
    q3Transform tmp = body->GetTransform();
    float x = tmp.position.x;
    float y = tmp.position.y;
    float z = tmp.position.z;
    
    Entity::setPosition(x, y, z);
    
    q3Vec3 axis;
    float angle;
    body->GetQuaternion().ToAxisAngle(&axis, &angle);
    if (q3LengthSq(axis) >= 0.0001)
        Entity::setRotation(-angle, Vector3(axis.x, axis.y, axis.z));
    
    //std::cout<<"position "<<x<<" "<<y<<" "<<z<<std::endl;
};

void Vehicle::setPosition(float x, float y, float z){
    Entity::setPosition(x, y, z);
    body->SetTransform(q3Vec3(x, y, z));
};

Vehicle::~Vehicle(){
    
};

// vehicle factory functions

Vehicle* VehicleFactory::createVehicle(q3Scene *scene){
    Vehicle* vehicle = new Vehicle();

    Mesh* mesh = new Mesh();
    mesh->createCube();
    
    q3BodyDef bodydef;
    bodydef.bodyType = eDynamicBody;
    q3Body* body = scene->CreateBody(bodydef);
    
    q3BoxDef boxDef;
    q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
    q3Identity(localSpace); // Specify the origin, and identity orientation
    
    // Create a box at the origin with width, height, depth = (1.0, 1.0, 1.0)
    // and add it to a rigid body. The transform is defined relative to the owning body
    boxDef.Set( localSpace, q3Vec3( 1.0, 1.0, 1.0 ));
    body->AddBox( boxDef );
    
    vehicle->setMesh(mesh);
    vehicle->setBody(body);
    
    return vehicle;
};


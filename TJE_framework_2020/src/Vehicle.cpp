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

void Vehicle::accelerate(float amount){
    
};

void Vehicle::turn(float amount){
    
};

void Vehicle::handbrake(){
    
};

void Vehicle::update(float elapsed_time){
    q3Transform tmp = body->GetTransform();
    float x = tmp.position.x;
    float y = tmp.position.y;
    float z = tmp.position.z;
    
    // TODO: add update the rotation of the object
    transform.setTranslation(x, y, z);
};


Vehicle* VehicleFactory::createVehicle(q3Scene *scene){
    Vehicle* vehicle = new Vehicle();

    Mesh* mesh = new Mesh();
    mesh->createCube();
    
    q3BodyDef bodydef;
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

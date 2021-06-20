//
//  PlayerController.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 13/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "PlayerCar.hpp"
#include "input.h"
#include "q3Factory.h"

PlayerCar::PlayerCar(q3Scene* scene){
    Mesh* mesh = Mesh::Get("data/carkit_v1.4/Models/OBJ format/sedan.obj");
    
    //mesh->createCube();

    q3Body* body = scene->CreateBody(createBodydef(eDynamicBody));
    body->AddBox(createBoxdef(1,1,1));
    
    setMesh(mesh);
    setBody(body);
};

void PlayerCar::update(float dt){
    Vehicle::update(dt);
    
    if (Input::isKeyPressed(SDL_SCANCODE_W)) Vehicle::accelerate(1);
    if (Input::isKeyPressed(SDL_SCANCODE_S)) Vehicle::accelerate(-1);
    
    if (Input::isKeyPressed(SDL_SCANCODE_A)) Vehicle::turn(1);
    if (Input::isKeyPressed(SDL_SCANCODE_D)) Vehicle::turn(-1);
};

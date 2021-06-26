//
//  EnemyCar.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 22/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "EnemyCar.hpp"
#include "q3Factory.h"

EnemyCar::EnemyCar(PlayerCar *player, std::string model_name, q3Scene *scene) : Vehicle(model_name, createBodydef(eDynamicBody), scene){
    this->player = player;
};

void EnemyCar::update(float dt){
    Vehicle::update(dt);
    
    Vector3 player_pos = player->getPosition();
    Vector3 player_dir = player_pos - getPosition();
    q3Vec3 player_dir_local = body->GetLocalVector(q3Vec3(player_dir.x, player_dir.y, player_dir.z));
    float steer_amount  = q3Clamp(-1,1, player_dir_local.x);
    turn(steer_amount);
    float accelerate_amount = q3Clamp(-1, 1, player_dir_local.z);
    accelerate(accelerate_amount);
};

void EnemyCar::setTraits(AiTraits t){
    traits = t;
    acceleration_strength = t.acceleration_strength;
    //turn_strength = t.turn_strength;
    max_forward_speed = t.max_forward_speed;
    // add rest later
};

//
//  EnemyCarManager.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 22/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "EnemyCarManager.hpp"
#include "framework.h"

EnemyCarManager::EnemyCarManager(PlayerCar *player, q3Scene *scene){
    this->player = player;
    this->scene = scene;
};

void EnemyCarManager::spawnCar(Vector3 position, AiTraits traits){
    EnemyCar *enemy = new EnemyCar(player, models.at(rand() % models.size()), scene);
    enemy->setPosition(position.x, position.y, position.z);
    enemy->setTraits(traits);
    enemies.push_back(enemy);
};

void EnemyCarManager::spawnCarAroundPlayer(float dst){
    Vector3 pos = player->getPosition();
    float angle = q3RandomFloat(0, PI*2);
    pos.x += std::cos(angle) * dst;
    pos.z += std::sin(angle) * dst;
    pos.y = 5;
    spawnCar(pos);
};

void EnemyCarManager::spawnCar(Vector3 position){
    spawnCar(position, generateAiTraits());
}

AiTraits EnemyCarManager::generateAiTraits(){
    AiTraits t;
    t.acceleration_strength = q3RandomFloat(20, 42);
    t.turn_strength = q3RandomFloat(8, 12);
    t.max_forward_speed = q3RandomFloat(20, 42);
    return t;
}

void EnemyCarManager::populate(){
    for (int i = enemies.size(); i < current_cars; i++){
        Vector3 pos;
        pos.random(map_size / 2);
        pos.y = 5;
        spawnCar(pos);
    }
};

void EnemyCarManager::addModel(std::string path){
    models.push_back(path);
};

void EnemyCarManager::update(float dt){
    std::vector<EnemyCar*> to_remove;
    
    for (EnemyCar *car : enemies){
        car->update(dt);
        //remove exploded car after N seconds
        if ((car->exploded && car->time_since_explosion > seconds_before_removing) || car->getPosition().y < -5)
            to_remove.push_back(car);
    }
    
    for (EnemyCar *car : to_remove) {
        std::remove(enemies.begin(), enemies.end(), car);
        //delete car;
    }
    to_remove.clear();
    
    populate();
    seconds_from_last_round += dt;
    if (seconds_from_last_round > seconds_per_round){
        current_cars = std::min(current_cars + 1, max_cars);
        seconds_from_last_round = 0;
    }
    
};

void EnemyCarManager::render(){
    for (EnemyCar *car : enemies)
        car->render();
};

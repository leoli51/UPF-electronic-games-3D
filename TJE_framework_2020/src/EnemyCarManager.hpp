//
//  EnemyCarManager.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 22/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef EnemyCarManager_hpp
#define EnemyCarManager_hpp

#include "EnemyCar.hpp"
#include "PlayerCar.hpp"

class EnemyCarManager {
public:
    std::vector<std::string> models;
    
    std::vector<EnemyCar*> enemies;
    PlayerCar *player;
    
    q3Scene *scene;
    
    float map_size = 100;
    int start_cars = 3;
    int current_cars = start_cars;
    int max_cars = 25;
    float seconds_from_last_round = 0;
    float seconds_per_round = 15;
    float seconds_before_removing = 3;
    
    EnemyCarManager(PlayerCar *player, q3Scene *scene);
    
    void spawnCar(Vector3 position, AiTraits traits);
    void spawnCar(Vector3 position);
    void spawnCarAroundPlayer(float dst);
    
    void populate();
    
    AiTraits generateAiTraits();
    
    void addModel(std::string path);
    
    void update(float dt);
    void render();
};

#endif /* EnemyCarManager_hpp */

//
//  Map.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 13/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef Map_hpp
#define Map_hpp

#include "entity.h"
#include "q3.h"
#include "PlayerCar.hpp"

class MapElement : public Entity {
public:
    q3Body *body;
    
    MapElement(std::string model_name, q3Scene *scene);
    
    void setPosition(float x, float y, float z);
};

class Map : public Entity {
public:
    q3Scene *scene;
    q3Body *body;
    
    std::vector<std::string> elements_models;
    std::vector<MapElement> elements;
    int num_elements;
    float max_dst_from_player;
    
    PlayerCar *player;
    
    Map(float size, int map_elements, q3Scene* scene, PlayerCar *player_car);
    
    
    void addElementModel(std::string path);
    void populate();
    
    void setPosition(float x, float y, float z);
    
    void update(float dt);
    void render();
    
};


#endif /* Map_hpp */

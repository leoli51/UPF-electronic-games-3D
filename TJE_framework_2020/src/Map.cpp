//
//  Map.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 13/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "Map.hpp"
#include "q3Factory.h"
#include "material.hpp"

MapElement::MapElement(std::string model_name, q3Scene *scene) : BodyEntity(createBodydef(eStaticBody), scene){
    setMesh(model_name);
};

Map::Map(float size, int map_elements, q3Scene* scene, PlayerCar *player_car){
    body = createBody(scene, createBodydef(eStaticBody));
    body->AddBox(createBoxdef(size, .1, size));
    mesh = new Mesh();
    mesh->createPlane(size / 2);
    
    Material *mat = new Material();
    mat->Kd = Vector4(223.0/255, 145.0/255, 94.0/255, 1);
    mat->name = "desert";
    mat->registerMaterial();
    
    num_elements = map_elements;
    max_dst_from_player = size * 0.4f;
    
    player = player_car;
    this->scene = scene;
};

void Map::addElementModel(std::string path){
    elements_models.push_back(path);
};

void Map::populate(){
    std::cout<<"[MAP] populating map with "<<num_elements<<" elements"<<std::endl;
    
    for (int i = elements.size(); i < num_elements; i++){
        MapElement el(elements_models.at(rand() % elements_models.size()), scene);
        
        float dst_from_player = q3RandomFloat(30, max_dst_from_player - 10);
        Vector3 pos;
        pos.random(1);
        pos.normalize();
        pos *= dst_from_player;
        
        el.setPosition(pos.x, 0, pos.z);
        el.setScale(2);
        
        elements.push_back(el);
    }
};

void Map::setPosition(float x, float y, float z){
    Entity::setPosition(x, y, z);
    body->SetTransform(q3Vec3(x, y, z));
};

void Map::update(float dt){
    setPosition(player->getPosition().x, 0, player->getPosition().z);
    
    int to_reposition = 0;
    for (MapElement &el : elements){
        el.update(dt);
        if (el.getPosition().distance(player->getPosition()) > max_dst_from_player){
            to_reposition++;
            // reposition object in front of player
            float reposition_dst = q3RandomFloat(max_dst_from_player / 2, max_dst_from_player - 10);
            float reposition_angle = q3RandomFloat(0,2*PI);
            
            
            Vector3 relative_pos(std::cos(reposition_angle), 0, std::sin(reposition_angle));
            relative_pos *= reposition_dst;
            
            Vector3 new_pos = relative_pos + player->getPosition();
            el.setPosition(new_pos.x, 0, new_pos.z); // set position at edge around player somewhere
        }
    }
};

void Map::render(){
    // render plane
    Shader::current->setUniform("u_model", transform);
    Material::Get("desert")->loadInShader();

    //do the draw call
    mesh->render( GL_TRIANGLES );
    
    // render map elements
    for (MapElement &el : elements)
        el.render();
};

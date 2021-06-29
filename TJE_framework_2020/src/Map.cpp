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
    body = createBody(scene, createBodydef(eStaticBody, (void*) this));
    body->AddBox(createBoxdef(size, .1, size));
    mesh = new Mesh();
    mesh->createPlane(size / 2);
    
    Material *mat = new Material();
    mat->Kd = Vector4(248.0/255, 248.0/255, 255.0/255, 1);
    mat->name = "map";
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
    
    generateWalls();
    
    for (int i = elements.size(); i < num_elements; i++){
        MapElement el(elements_models.at(rand() % elements_models.size()), scene);
        
        //float dst_from_player = q3RandomFloat(30, max_dst_from_player - 10);
        Vector3 pos;
        //pos.random(1);
        //pos.normalize();
        //pos *= dst_from_player;
        pos.random(mesh->box.halfsize.x);
        el.setPosition(pos.x, 0, pos.z);
        el.setScale(20);
        
        elements.push_back(el);
    }
};

void Map::generateWalls(){
    float hs = mesh->box.halfsize.x;
    
    q3Transform tl;
    q3Identity(tl);
    tl.position = q3Vec3(-hs, 0, 0);
    tl.rotation.Set(q3Vec3(0,0,1), M_PI_2);
    
    q3Transform tu;
    q3Identity(tu);
    tu.position = q3Vec3(0, 0, hs);
    tu.rotation.Set(q3Vec3(1,0,0), M_PI_2);

    
    q3Transform tr;
    q3Identity(tr);
    tr.position = q3Vec3(hs, 0, 0);
    tr.rotation.Set(q3Vec3(0,0,1), M_PI_2);
    
    q3Transform td;
    q3Identity(td);
    td.position = q3Vec3(0, 0, -hs);
    td.rotation.Set(q3Vec3(1,0,0), M_PI_2);

    q3BoxDef bd;
    q3Vec3 extents(hs*2, .1, hs*2);
    
    bd.Set(tl, extents);
    body->AddBox(bd);
    bd.Set(tu, extents);
    body->AddBox(bd);
    bd.Set(tr, extents);
    body->AddBox(bd);
    bd.Set(td, extents);
    body->AddBox(bd);
    
    walls[0].setPosition(-hs, 0, 0);
    walls[0].setRotation(M_PI_2, Vector3(0,0,1));
    
    walls[1].setPosition(0, 0, hs);
    walls[1].setRotation(M_PI_2, Vector3(1,0,0));
    
    walls[2].setPosition(hs, 0, 0);
    walls[2].setRotation(M_PI_2, Vector3(0,0,1));
    
    walls[3].setPosition(0, 0, -hs);
    walls[3].setRotation(M_PI_2, Vector3(1,0,0));
    
    for (int i = 0; i < 4; i++){
        walls[i].setMesh(mesh);
        walls[i].default_kd.set(.5,.5,.5,.5);
    }
};

void Map::setPosition(float x, float y, float z){
    Entity::setPosition(x, y, z);
    body->SetTransform(q3Vec3(x, y, z));
};

void Map::update(float dt){
    //setPosition(player->getPosition().x, 0, player->getPosition().z);
    
    //int to_reposition = 0;
    for (MapElement &el : elements){
        el.update(dt);
    //    if (el.getPosition().distance(player->getPosition()) > max_dst_from_player){
    //        to_reposition++;
            // reposition object in front of player
    //        float reposition_dst = q3RandomFloat(max_dst_from_player / 2, max_dst_from_player - 10);
    //        float reposition_angle = q3RandomFloat(0,2*PI);
            
            
    //        Vector3 relative_pos(std::cos(reposition_angle), 0, std::sin(reposition_angle));
    //        relative_pos *= reposition_dst;
            
    //        Vector3 new_pos = relative_pos + player->getPosition();
    //        el.setPosition(new_pos.x, 0, new_pos.z); // set position at edge around player somewhere
    //    }
    }
};

void Map::render(){
    // render plane
    Shader::current->setUniform("u_model", transform);
    Material::Get("map")->loadInShader();

    //do the draw call
    mesh->render( GL_TRIANGLES );
    for (int i = 0; i < 4; i++)
        walls[i].render();
    
    // render map elements
    for (MapElement &el : elements)
        el.render();
};

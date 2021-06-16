//
//  entity.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 11/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "entity.h"

Entity::Entity(){
    
};


void Entity::setMesh(Mesh *mesh){
    this->mesh = mesh;
};

void Entity::setMesh(std::string mesh_path){
    mesh = Mesh::Get(mesh_path.c_str());
};

void Entity::update(float elapsed_time){
    
};

void Entity::render(Shader* shader){
    if (mesh == NULL) return;
    
    //upload uniforms
    shader->setUniform("u_color", Vector4(1,1,1,1));
    //shader->setUniform("u_texture", texture, 0);
    shader->setUniform("u_model", transform);
    
    //do the draw call
    mesh->render( GL_TRIANGLES );

};

void Entity::setPosition(float x, float y, float z){
    //transform.translateGlobal(x,y,z);
    transform.setTranslation(x, y, z);
};

void Entity::setRotation(float angle, Vector3 axis){
    Vector3 tmp = transform.getTranslation();
    transform.setIdentity();
    transform.rotate(angle, axis);
    transform.translateGlobal(tmp.x, tmp.y, tmp.z);
    //transform.setRotation(angle, axis);
};

Vector3 Entity::getPosition(){
    return transform.getTranslation();
};

Matrix44 Entity::getRotation(){
    return transform.getRotationOnly();
};

Entity::~Entity(){
    
};

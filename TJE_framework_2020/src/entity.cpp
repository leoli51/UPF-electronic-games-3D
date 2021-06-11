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

void Entity::render(Shader* shader){
    //upload uniforms
    shader->setUniform("u_color", Vector4(1,1,1,1));
    //shader->setUniform("u_texture", texture, 0);
    shader->setUniform("u_model", transform);
    
    //do the draw call
    mesh->render( GL_TRIANGLES );

};

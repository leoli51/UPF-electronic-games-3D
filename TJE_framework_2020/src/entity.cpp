//
//  entity.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 11/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "entity.h"
#include "material.hpp"

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

void Entity::render(){
    if (mesh == NULL) return;
    
    Shader::current->setUniform("u_model", transform);
    
    if (mesh->getNumSubmeshes() > 0){
        for (int submesh_id = 0; submesh_id < mesh->getNumSubmeshes(); submesh_id++){
            std::string material_name = mesh->submeshes.at(submesh_id).material;
            Material* material = Material::Get(material_name);
            if (material != NULL){
                material->loadInShader();
                //Shader::current->setUniform("u_color", material->Kd);
            }
            mesh->render(GL_TRIANGLES, submesh_id);
        }
    }
    else {
        //upload uniforms
        Shader::current->setUniform("Kd", Vector4(1,1,1,1));
        //do the draw call
        mesh->render( GL_TRIANGLES );
    }

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

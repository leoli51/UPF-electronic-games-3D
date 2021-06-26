//
//  bodyentity.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 22/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "bodyentity.hpp"
#include "q3Factory.h"

BodyEntity::BodyEntity(q3BodyDef def, q3Scene *scene){
    this->scene = scene;
    body = scene->CreateBody(def);
};

void BodyEntity::setMesh(std::string model_path){
    if (box != NULL) body->RemoveBox(box);
    Entity::setMesh(model_path);
    generateColliderFromMesh();
};

void BodyEntity::setMesh(Mesh *mesh){
    if (box != NULL) body->RemoveBox(box);
    Entity::setMesh(mesh);
    generateColliderFromMesh();
};

void BodyEntity::generateColliderFromMesh(){
    assert(mesh != NULL && "Missing mesh");
    
    const float collider_scale = .9f;
    Vector3 size = getScale() * mesh->box.halfsize * 2 * collider_scale;
    box = body->AddBox(createBoxdef(size.x, size.y, size.z));
};

void BodyEntity::setPosition(float x, float y, float z){
    Entity::setPosition(x, y, z);
    body->SetTransform(q3Vec3(x, y, z));
};

void BodyEntity::setRotation(float angle, Vector3 axis){
    Entity::setRotation(angle, axis);
    body->SetTransform(body->GetTransform().position, q3Vec3(axis.x,axis.y,axis.z), angle);
};

void BodyEntity::setScale(float x, float y, float z){
    Entity::setScale(x, y, z);
    if (box != NULL) body->RemoveBox(box);
    generateColliderFromMesh();
};

void BodyEntity::setScale(float scale){
    setScale(scale, scale, scale);
};

void BodyEntity::update(float dt){
    q3Transform tmp = body->GetTransform();
    float x = tmp.position.x;
    float y = tmp.position.y;
    float z = tmp.position.z;
    
    Entity::setPosition(x, y, z);
    
    q3Vec3 axis;
    float angle;
    body->GetQuaternion().ToAxisAngle(&axis, &angle);
    if (q3LengthSq(axis) >= 0.0001)
        Entity::setRotation(-angle, Vector3(axis.x, axis.y, axis.z));
};

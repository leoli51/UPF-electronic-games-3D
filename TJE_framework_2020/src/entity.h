
#ifndef ENTITY_H
#define ENTITY_H

#include "framework.h"
#include "mesh.h"
#include <string>
#include "shader.h"

class Entity
{
public:
    Entity(); //constructor
    virtual ~Entity(); //destructor

    //some attributes 
    std::string name;
    
    Matrix44 transform;
    Mesh* mesh = NULL;
    
    void setMesh(Mesh* mesh);
    void setMesh(std::string mesh_path);

    //methods overwritten by derived classes 
    virtual void render();
    virtual void update(float elapsed_time);

    //some useful methods...
    virtual void setPosition(float x, float y, float z);
    virtual void setRotation(float angle, Vector3 axis);
    
    Vector3 getPosition();
    Matrix44 getRotation();
};

#endif

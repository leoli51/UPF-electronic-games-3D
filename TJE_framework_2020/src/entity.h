
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
    Mesh* mesh;
    
    void setMesh(Mesh* mesh);
    void setMesh(std::string mesh_path);

    //methods overwritten by derived classes 
    virtual void render(Shader* shader);
    virtual void update(float elapsed_time);

    //some useful methods...
    Vector3 getPosition();
};

#endif

//
//  bodyentity.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 22/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef bodyentity_hpp
#define bodyentity_hpp

#include "entity.h"
#include "q3.h"
#include "framework.h"

class BodyEntity : public Entity {
public:
    q3Scene *scene = NULL;
    q3Body *body = NULL;
    const q3Box *box = NULL;

    BodyEntity(q3BodyDef def, q3Scene *scene);
    
    void setMesh(std::string model_path);
    void setMesh(Mesh *mesh);
    
    void setPosition(float x, float y, float z);
    void setRotation(float angle, Vector3 axis);
    void setScale(float x, float y, float z);
    void setScale(float scale);
    void update(float dt);
    
protected:
    void generateColliderFromMesh();
};

#endif /* bodyentity_hpp */

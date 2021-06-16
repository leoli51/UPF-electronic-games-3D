//
//  q3Factory.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 16/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "q3Factory.h"


q3BodyDef createBodydef(q3BodyType type, float gravityscale, float lineardamping, float angulardamping){
    q3BodyDef bodydef;
    bodydef.gravityScale = gravityscale;
    bodydef.bodyType = type;
    bodydef.linearDamping = lineardamping;
    bodydef.angularDamping = angulardamping;
    
    return bodydef;
};

q3Body* createBody(q3Scene* scene, q3BodyDef bodydef){
    q3Body* body = scene->CreateBody(bodydef);
    
    q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
    q3Identity(localSpace); // Specify the origin, and identity orientation
    return body;
};

q3BoxDef createBoxdef(float width, float height, float depth){
    q3BoxDef boxdef;
    q3Transform localSpace; // Contains position and orientation, see q3Transform.h for details
    q3Identity(localSpace); // Specify the origin, and identity orientation
    
    // Create a box at the origin with width, height, depth = (1.0, 1.0, 1.0)
    // and add it to a rigid body. The transform is defined relative to the owning body
    boxdef.Set(localSpace, q3Vec3( width, height, depth ));
    return boxdef;
};

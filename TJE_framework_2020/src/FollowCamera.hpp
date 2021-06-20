//
//  FollowCamera.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 20/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef FollowCamera_hpp
#define FollowCamera_hpp

#include "camera.h"
#include "entity.h"
#include "framework.h"

class FollowCamera : public Camera {
public:
    Matrix44 *target_transform;
    
    float distance_to_target = 10;
    float pitch = PI/6;
    float lerp_speed = 4;
    
    void setLerpSpeed(float ls);
    void setDistanceToTarget(float zoom);
    void setPitch(float angle);
    void setTarget(Matrix44* target);
    
    void update(float dt);
    
private:
    Vector3 computeEyePosition();
    Vector3 computeCenterPosition();
};

#endif /* FollowCamera_hpp */

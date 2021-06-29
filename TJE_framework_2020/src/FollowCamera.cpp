//
//  FollowCamera.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 20/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "FollowCamera.hpp"

void FollowCamera::setLerpSpeed(float ls){
    lerp_speed = ls;
};

void FollowCamera::setDistanceToTarget(float zoom){
    distance_to_target = zoom;
};

void FollowCamera::setPitch(float angle){
    pitch = angle;
};

void FollowCamera::setTarget(Matrix44 *target){
    target_transform = target;
};

void FollowCamera::update(float dt){
    Camera::eye = lerp(eye, computeEyePosition(), clamp(lerp_speed));
    Camera::center = computeCenterPosition();//lerp(center, computeCenterPosition(), clamp(lerp_speed * dt));
    updateViewMatrix();
};

Vector3 FollowCamera::computeEyePosition(){
    Vector3 new_eye = target_transform->getTranslation();
    Vector3 x_delta = - std::cos(pitch) * distance_to_target * target_transform->frontVector();
    Vector3 y_delta = std::sin(pitch) * distance_to_target * target_transform->topVector();
    
    new_eye = new_eye + x_delta + y_delta;
    return new_eye;
};

Vector3 FollowCamera::computeCenterPosition(){
    Vector3 new_center = target_transform->getTranslation();
    return new_center;
};

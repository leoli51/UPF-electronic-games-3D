//
//  ContactListener.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 23/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "ContactListener.hpp"
#include "q3Contact.h"
#include "Vehicle.hpp"

void ContactListener::BeginContact(const q3ContactConstraint *contact){
    // for now only map and vehicles have userdata .. TODO implement better system
    //int eStatic = 0x020;
    int eDynamic = 0x040;
    //int eKinematic = 0x080;
    
    Vehicle *vehicleA = (contact->bodyA->GetFlags() & eDynamic) ? (Vehicle*) contact->bodyA->GetUserData() : NULL;
    Vehicle *vehicleB = (contact->bodyB->GetFlags() & eDynamic) ? (Vehicle*) contact->bodyB->GetUserData() : NULL;
    
    q3Vec3 contact_position = contact->manifold.contacts[0].position;
    
    q3Vec3 impact_dir_A = (contact_position - contact->bodyA->GetTransform().position);
    float impact_speed_A = q3Dot(contact->bodyA->GetLinearVelocity(), impact_dir_A);
    q3Vec3 impact_dir_B = (contact_position - contact->bodyB->GetTransform().position);
    float impact_speed_B = q3Dot(contact->bodyB->GetLinearVelocity(), impact_dir_B);
    
    float impact_speed = impact_speed_A + impact_speed_B;

    if (vehicleA){
        //std::cout<<"impact A: "<<impact_speed_A<<std::endl;
        if (impact_speed > vehicleA->explosion_speed)
            vehicleA->explode();
    }
    if (vehicleB){
        //std::cout<<"impact B: "<<impact_speed_B<<std::endl;
        if (impact_speed > vehicleB->explosion_speed)
            vehicleB->explode();
    }
};

void ContactListener::EndContact(const q3ContactConstraint *contact){
};

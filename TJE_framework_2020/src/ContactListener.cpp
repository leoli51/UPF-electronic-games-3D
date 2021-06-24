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
    int eStatic = 0x020;
    int eDynamic = 0x040;
    int eKinematic = 0x080;
    
    Vehicle *vehicle = NULL;
    if ((contact->bodyA->GetFlags() & eDynamic) && (contact->bodyB->GetFlags() & eStatic)){
        vehicle = (Vehicle*) contact->bodyA->GetUserData();
    }
    
    else if ((contact->bodyB->GetFlags() & eDynamicBody) && (contact->bodyA->GetFlags() & eStaticBody)){
        vehicle = (Vehicle*) contact->bodyB->GetUserData();
    }
    
    if (vehicle != NULL){
        std::cout<<"Vehicle ground collision"<<std::endl;
        vehicle->on_ground = true;
    }
};

void ContactListener::EndContact(const q3ContactConstraint *contact){
    // for now only map and vehicles have userdata .. TODO implement better system
    Vehicle *vehicle = NULL;
    if ((contact->bodyA->GetFlags() & eDynamicBody) && (contact->bodyB->GetFlags() & eStaticBody)){
        vehicle = (Vehicle*) contact->bodyA->GetUserData();
    }
    
    else if ((contact->bodyB->GetFlags() & eDynamicBody) && (contact->bodyA->GetFlags() & eStaticBody)){
        vehicle = (Vehicle*) contact->bodyB->GetUserData();
    }
    
    if (vehicle != NULL){
        vehicle->on_ground = false;
    }
};

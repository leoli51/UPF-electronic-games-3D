//
//  EnemyCar.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 22/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef EnemyCar_hpp
#define EnemyCar_hpp

#include "Vehicle.hpp"
#include "PlayerCar.hpp"


struct AiTraits {
    float max_forward_speed;
    float acceleration_strength;
    float turn_strength;
    float skidding_velocity;
    float max_impact_speed;
};

class EnemyCar : public Vehicle {
public:
    PlayerCar *player;
    AiTraits traits;
    
    EnemyCar(PlayerCar *player, std::string model_name, q3Scene *scene);
    
    void update(float dt);
    
    void setTraits(AiTraits t);
};
#endif /* EnemyCar_hpp */

//
//  Map.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 13/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef Map_hpp
#define Map_hpp

#include "entity.h"
#include "q3.h"

class Map : public Entity {
public:
    q3Body* body;
    
    Map(q3Scene* scene);
    
    void setPosition(float x, float y, float z);
    void setSize(float size);
};


#endif /* Map_hpp */

//
//  UIElement.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 09/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef UIElement_hpp
#define UIElement_hpp

#include "framework.h"

class UIElement {
public:
    Vector2 pivot;
    Vector2 position;
    Vector2 size;
        
    virtual void update(float dt);
    virtual void render();
};

#endif /* UIElement_hpp */

//
//  stage.h
//  TJE_XCODE
//
//  Created by Laura Mac on 08/05/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef stage_h
#define stage_h

#include "camera.h"
#include <map>

class Stage {
public:
    virtual void init();
    virtual void update(float dt);
    virtual void render();
    virtual void deinit();
};

class StageManager {
private:
    std::map<std::string, Stage*> loaded_stages;

public:
    //TODO continue implementation...
    
}

#endif /* stage_h */

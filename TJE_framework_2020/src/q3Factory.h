//
//  q3Factory.h
//  TJE_XCODE
//
//  Created by Laura Mac on 13/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef q3Factory_h
#define q3Factory_h

#include "q3.h"

q3BodyDef createBodydef(q3BodyType type, float gravityscale = 1.0, float lineardamping = 0.0, float angulardamping = 0.1);
q3Body* createBody(q3Scene* scene, q3BodyDef bodydef);
q3BoxDef createBoxdef(float width, float height, float depth);


#endif /* q3Factory_h */

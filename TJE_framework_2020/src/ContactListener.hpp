//
//  ContactListener.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 23/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef ContactListener_hpp
#define ContactListener_hpp

#include "q3.h"

class ContactListener : public q3ContactListener
{
public:
    virtual ~ContactListener( )
    {
    }
    
    void BeginContact( const q3ContactConstraint *contact );
    void EndContact( const q3ContactConstraint *contact );
};

#endif /* ContactListener_hpp */

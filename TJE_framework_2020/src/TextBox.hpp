//
//  TextBox.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 09/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef TextBox_hpp
#define TextBox_hpp

#include "UIElement.hpp"
#include <string>

class TextBox : public UIElement {
public:
    std::string text;
    
    TextBox();
    
    void setText(std::string new_text);

};

#endif /* TextBox_hpp */

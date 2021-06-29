//
//  material.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 17/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include <map>
#include <string>

#include "framework.h"

class Material {
public:
    static std::map<std::string, Material*> sMaterialsLoaded;
    static Material* Get(std::string material_name);
    static bool loadMTL(const char *filename);
    
    std::string name;
    
    Vector4 Ka;
    Vector4 Kd;
    Vector4 Ks;

    Material();
    
    void loadInShader();
    void registerMaterial();
    
};

#endif /* material_hpp */

//
//  material.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 17/06/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "material.hpp"
#include "utils.h"
#include "shader.h"

#include <sys/stat.h>
#include <iostream>

std::map<std::string, Material*> Material::sMaterialsLoaded;

Material::Material(){
    name = "noname";
    Kd = Vector4(1,1,1,1);
    Ka = Vector4(1,1,1,1);
    Ks = Vector4(1,1,1,1);
};

void Material::loadInShader(){
    Shader::current->setUniform("Kd", Kd);
};

void Material::registerMaterial(){
    Material::sMaterialsLoaded[this->name] = this;
};


Material* Material::Get(std::string material_name){
    std::map<std::string, Material*>::iterator it = sMaterialsLoaded.find(material_name);
    if (it != sMaterialsLoaded.end())
        return it->second;
    //std::cerr<<"[Material Get]: No material with this name "<<material_name<<std::endl;
    return NULL;
};

bool Material::loadMTL(const char* filename)
{
    struct stat stbuffer;
    
    FILE* f = fopen(filename,"rb");
    if (f == NULL)
    {
        std::cerr << "File not found: " << filename << std::endl;
        return false;
    }
    
    stat(filename,&stbuffer);
    
    unsigned int size = stbuffer.st_size;
    char* data = new char[size+1];
    fread(data,size,1,f);
    fclose(f);
    data[size] = 0;
    
    char* pos = data;
    char line[255];
    int i = 0;
    
    Material *current_material = NULL;
    
    //parse file
    while(*pos != 0)
    {
        if (*pos == '\n') pos++;
        if (*pos == '\r') pos++;
        
        //read one line
        i = 0;
        while(i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
        memcpy(line,pos,i);
        line[i] = 0;
        pos = pos + i;
        
        //std::cout << "Line: \"" << line << "\"" << std::endl;
        if (*line == '#' || *line == 0) continue; //comment
        
        //tokenize line
        std::vector<std::string> tokens = tokenize(line," ");
        
        if (tokens.empty()) continue;
        
        if (tokens[0] == "newmtl")
        {
            // new material
            std::cout<<"[MATERIAL loadMTL]: found material definition with name: "<<tokens[1]<<std::endl;
            current_material = new Material();
            current_material->name = tokens[1];
            Material::sMaterialsLoaded[tokens[1]] = current_material;
        }
        else if (tokens[0] == "Kd")
        {
            // retrieve diffusion color
            float r = std::stof(tokens[1]);
            float g = std::stof(tokens[2]);
            float b = std::stof(tokens[3]);
            
            current_material->Kd = Vector4(r,g,b,1);
        }
        else if (tokens[0] == "other mtl files attributes" )
        {
          // store them in current_material
        }
    }
    return true;
};

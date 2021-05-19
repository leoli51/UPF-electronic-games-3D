//
//  citymap.cpp
//  TJE_XCODE
//
//  Created by Laura Mac on 17/05/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#include "citymap.hpp"
#include "tinyxml2.h"
#include <iostream>

void CityMap::loadTileSetData(std::string xml_file, std::string models_dir){
    // open the xml document with the tileset data
    tinyxml2::XMLDocument doc;
    doc.LoadFile(xml_file.c_str());
    
    // load the connections node
    tinyxml2::XMLElement* connections = doc.FirstChildElement("tileset")->FirstChildElement("connections");
    // get the number of connections
    int connections_size = connections->IntAttribute("size");
    
    // delete previous tileset data
    deleteTileSet();
    
    // get first connection
    tinyxml2::XMLElement* connection = connections->FirstChildElement();
    
    for (int i = 0; i < connections_size; i++){
        ConnectionData* tmp = new ConnectionData();
        tmp->name = connection->Attribute("name");
        tmp->id = connection->IntAttribute("id");
        tmp->mask = connection->IntAttribute("flag");
        connectionset[tmp->name] = tmp;
        
        // get next connection
        connection = connection->NextSiblingElement();
    }
    
    // prepare tileset vector to hold enough enough information to contain all the tiledata
    tinyxml2::XMLElement* tiles = doc.FirstChildElement("tileset")->FirstChildElement("tiles");
    int tileset_size = tiles->IntAttribute("size");
    
    
    // get first tile
    tinyxml2::XMLElement* tile = tiles->FirstChildElement();
    for (int i = 0; i < tileset_size; i++){
        // populate connection vector
        TileData* tmp = new TileData();
        tmp->name = tile->Attribute("name");
        // load model mesh
        tmp->model = tile->Attribute("model");
        if (tmp->model == "NONE") tmp->mesh = NULL;
        else tmp->mesh = Mesh::Get((models_dir + tmp->model).c_str());
        
        for (std::string conn : {"up", "right", "down", "left"})
            tmp->connections[conn] = connectionset[tile->Attribute(conn.c_str())];
        
        tileset[tmp->name] = tmp;
        
        // get next connection
        tile = tile->NextSiblingElement();
    }

    // print for testing purposes:
    std::cout<<"loaded tiles: "<<std::endl;
    std::map<std::string, TileData*>::iterator it;
    for(it = tileset.begin(); it != tileset.end(); ++it)
        std::cout<<it->first<<std::endl;
    std::cout<<"loaded connections: "<<std::endl;
    std::map<std::string, ConnectionData*>::iterator it2;
    for(it2 = connectionset.begin(); it2 != connectionset.end(); ++it2)
        std::cout<<it2->first<<std::endl;
    
};

void CityMap::setSize(int size){
    deleteGrid();
    this->size = size;
    
    grid = new Tile**[size];
    for (int i = 0; i < size; i++)
        grid[i] = new Tile*[size];
};

void CityMap::deleteGrid(){
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            delete grid[i][j];
        delete [] grid[i];
    }
    delete [] grid;
};

Tile* CityMap::makeTile(std::string name){
    Tile* tile = new Tile();
    tile->data = tileset[name];
    return tile;
};

bool CityMap::generateMap(){
    // this method tries to generate a map of the given size using the WFC algorithm (tileset version)
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++){
            grid[r][c] = makeTile("road_bend");
        }
    
    return true;
};

void CityMap::deleteTileSet(){
    std::map<std::string, TileData*>::iterator it;
    for(it = tileset.begin(); it != tileset.end(); ++it)
        delete it->second;
    std::map<std::string, ConnectionData*>::iterator it_c;
    for(it_c = connectionset.begin(); it_c != connectionset.end(); ++it_c)
        delete it->second;
    
    tileset.clear();
    connectionset.clear();
};

CityMap::~CityMap(){
    deleteGrid();
    deleteTileSet();
};



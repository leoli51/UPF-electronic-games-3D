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
    
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            grid[x][y] = nullptr;
};

Tile* CityMap::makeTile(std::string name){
    Tile* tile = new Tile();
    tile->data = tileset[name];
    return tile;
};

bool CityMap::generateMap(){
    // this method tries to generate a map of the given size using the WFC algorithm (tileset version)
    // for each position see which connections are needed
    // get set of tiles that satisfies connections
    // choose one at random, repeat
    
    // borders are filled with "empty" tiles for easing control purposes
    for (int i = 1; i < size - 1; i++){
        grid[i][0] = makeTile("empty");
        grid[0][i] = makeTile("empty");
        grid[size - 1][i] = makeTile("empty");
        grid[i][size - 1] = makeTile("empty");
    }
    grid[size - 1][0] = makeTile("empty");
    grid[size - 1][size - 1] = makeTile("empty");
    grid[0][size -1] = makeTile("empty");
    grid[0][0] = makeTile("empty");
    
    for (int x = 1; x < size - 1; x++){
        for (int y = 1; y < size - 1; y++){
            ConnectionData* connections[4];
            connections[0] = grid[y - 1][x]->data->connections["down"];
            connections[1] = grid[y][x + 1]->data->connections["left"];
            connections[2] = grid[y + 1][x]->data->connections["up"];
            connections[3] = grid[y][x - 1]->data->connections["right"];
            // todo add any connnection 
        }
    }

    
    
    // second look for the connections
    for (int off_x = -1; off_x <= 1; off_x++)
        for (int off_y = -1; off_y <=1; off_y++)
            if (off_x != 0 && off_y != 0)
                continue;
            else {
                int test_x = curr_x + off_x;
                int test_y = curr_y + off_y;
                if (!(0 <= test_x && test_x < size)) continue;
                if (!(0 <= test_y && test_x < size)) continue;
                if (grid[test_y][test_x] != nullptr) continue;
                
                Tile* last = grid[curr_y][curr_x];
                // 0
                ConnectionData* curr_connection = last->data->connections[];
            }
    
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++){
            grid[r][c] = makeTile("road_straight");
            
        }
    
    return true;
};

void CityMap::render(Shader* shader_program){
    Matrix44 m;
    m.setTranslation(0,0,0);
    
    //do the draw call
    for (int x = 0; x < size; x++){
        for (int y = 0; y < size; y++){
            m.setTranslation(x,0,y);
            shader_program->setUniform("u_model", m);
            Tile* tile = getTileAt(x, y);
            if (tile->data->model != "NONE")
                tile->data->mesh->render(GL_TRIANGLES);
        }
    }
};

Tile* CityMap::getTileAt(int x, int y){
    return grid[y][x];
};

// destruction and private methods

void CityMap::deleteGrid(){
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            delete grid[i][j];
        delete [] grid[i];
    }
    delete [] grid;
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



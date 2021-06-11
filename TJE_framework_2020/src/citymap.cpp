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
        
        int conn_index = 0;
        for (std::string conn_name : {"up", "right", "down", "left"}){
            tmp->connections[conn_index] = connectionset[tile->Attribute(conn_name.c_str())];
            conn_index++;
        }
    
        
        tileset[tmp->name] = tmp;
        
        // get next connection
        tile = tile->NextSiblingElement();
    }
    
    // add the ANY conncection
    ConnectionData* ANY = new ConnectionData();
    ANY->name = "ANY";
    ANY->id = -1;
    ANY->mask = 0xFF;
    connectionset[ANY->name] = ANY;
    
    // add the NONE tile
    TileData* NONE = new TileData();
    NONE->name = "NONE";
    NONE->model = "NONE";
    NONE->mesh = NULL;
    for (int conn_index = 0; conn_index < 4; conn_index++)
        NONE->connections[conn_index] = connectionset["ANY"];
    tileset[NONE->name] = NONE;
    
    delete NONE_TILE;
    NONE_TILE = makeTile("NONE");
    
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

Tile* CityMap::makeTile(std::string name, int rotation){
    Tile* tile = new Tile();
    tile->data = tileset[name];
    tile->rotation = rotation;
    return tile;
};

bool CityMap::generateMap(){
    // this method tries to generate a map of the given size using the WFC algorithm (tileset version)
    // for each position see which connections are needed
    // get set of tiles that satisfies connections
    // choose one at random
    // repeat
    
    // inside of map is filled with "ANY" tiles
    
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            grid[i][j] = NONE_TILE;
        }
    }
    
    // iterate through tiles and generate map
    for (int x = 1; x < size - 1; x++){
        for (int y = 1; y < size - 1; y++){
            ConnectionData* connections[4];
            
            connections[0] = grid[y - 1][x]->getRotatedConnection(2);
            connections[1] = grid[y][x + 1]->data->connections[3];
            connections[2] = grid[y + 1][x]->data->connections[0];
            connections[3] = grid[y][x - 1]->data->connections[1];
            // todo add "ANY" connnection
            
            // look for compatible tiles
            std::vector<Tile*>* compatible_tiles = getCompatibleTiles(connections);
            std::cout<<"there are "<<compatible_tiles->size()<<" compatible tiles for pos "<<x<<" "<<y<<std::endl;
            Tile* chosen = compatible_tiles->empty() ? makeTile("NONE") : compatible_tiles->at(std::rand() % compatible_tiles->size());
            
            //  TODO: free unused tiles! and tile vector
            std::cout<<chosen->data->name<<" was chosen. with rotation: "<<chosen->rotation<<std::endl;
            grid[y][x] = chosen;
        }
    }
    
    return true;
};

// optimize to return pointer or something like this
std::vector<Tile*>* CityMap::getCompatibleTiles(ConnectionData* connections[4]){
    std::vector<Tile*>* tiles = new std::vector<Tile*>();
    
    for (auto const& it : tileset){
        if (it.first == "NONE") continue;
        for (int rotation_offset = 0; rotation_offset < 4; rotation_offset++){
            bool fits = true;
            for (int connection_index = 0; connection_index < 4; connection_index++){
                if (it.second->connections[(rotation_offset + connection_index) % 4]->id != connections[connection_index]->id && connections[connection_index]->id != -1){
                    fits = false; // tile doesnt fit with this rotation lets try next rotation
                    break;
                }
            }
                if (fits){ // if the tile fits with this rotation add it to the possible tiles
                    tiles->push_back(makeTile(it.first, rotation_offset));
                    std::cout<<it.first<<" fits with rotation: "<<rotation_offset<<std::endl;
                }
            }
        }
    
    return tiles;
};


void CityMap::render(Shader* shader_program){
    Matrix44 m;
    m.setTranslation(0,0,0);
    static const Vector3 rotation_axis(0,1,0);
    
    //do the draw call
    for (int x = 0; x < size; x++){
        for (int y = 0; y < size; y++){
            Tile* tile = getTileAt(x, y);
            
            m.setIdentity();
            m.rotateGlobal(PI / 2 * tile->rotation, rotation_axis);
            m.translateGlobal(x,0,y);
            
            shader_program->setUniform("u_model", m);
            
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

ConnectionData* Tile::getRotatedConnection(int dir){
    return data->connections[(dir + rotation) % 4];
};




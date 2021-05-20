//
//  citymap.hpp
//  TJE_XCODE
//
//  Created by Laura Mac on 17/05/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef citymap_hpp
#define citymap_hpp

#include <string>
#include <vector>
#include <map>
#include "mesh.h"
#include "shader.h"

class Tile;
class TileData;
class ConnectionData;

class CityMap {
    
public:
    Tile*** grid;
    int size; // lets keep it simple... only square grids
    
    std::map<std::string, TileData*> tileset;
    std::map<std::string, ConnectionData*> connectionset;
    
    void loadTileSetData(std::string xml_file, std::string models_dir);
    
    Tile* makeTile(std::string name);
    
    void setSize(int size);
    bool setTileAt(TileData* tile_type, int x, int y);
    Tile* getTileAt(int x, int y);
    
    bool generateMap();
    
    void render(Shader* shader_program);
    
    ~CityMap();
    
private:
    void deleteGrid();
    void deleteTileSet();
};

class TileData { // class used to store information about each tile type
public:
    int type;
    std::map<std::string, ConnectionData*> connections;
    std::string name;
    std::string model;
    Mesh* mesh;
    
};

class ConnectionData {
public:
    std::string name;
    int id;
    int mask;
};

class Tile { // class used to store information about each tile
public:
    TileData* data; // use only one tiledata instance for all the tiles!
    int rotation;
};

#endif /* citymap_hpp */

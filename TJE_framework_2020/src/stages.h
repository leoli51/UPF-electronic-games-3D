//
//  stages.h
//  TJE_XCODE
//
//  Created by Laura Mac on 10/05/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef stages_h
#define stages_h

#include <sstream>
#include <iomanip>

#include "stage.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "framework.h"
#include "q3.h"
#include "Vehicle.hpp"
#include "Map.hpp"
#include "PlayerCar.hpp"
#include "FollowCamera.hpp"
#include "EnemyCarManager.hpp"
#include "ContactListener.hpp"


class TestStage : public Stage {
public:
    bool mouse_locked = false;
    float speed = 2;
    FollowCamera* camera;
    q3Scene* scene;
    PlayerCar* player;
    EnemyCarManager* aiManager;
    ContactListener* contact_listener;
    Map* map;
    Shader* shader;
    
    float seconds_passed = 0;
    float restart_timer = 5;
    
    void init(){
        //OpenGL flags
        glEnable( GL_CULL_FACE ); //render both sides of every triangle
        glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
        
        //create our camera
        camera = new FollowCamera();
        camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
        
        camera->setPerspective(70.f, Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); //set the projection, we want to be perspective
        
        //hide the cursor
        SDL_ShowCursor(!mouse_locked); //hide or show the mouse
        
        // create our physics world
        scene = new q3Scene(1.0/60.0, q3Vec3(0,-10,0));
        shader = Shader::Get("data/shaders/basic.vs", "data/shaders/basic.fs");

        player = new PlayerCar("data/carkit_v1.4/Models/OBJ format/sedan.obj",scene);
        player->setPosition(0,10,0);
        
        aiManager = new EnemyCarManager(player, scene);
        aiManager->map_size = 200;
        aiManager->addModel("data/carkit_v1.4/Models/OBJ format/police.obj");
        aiManager->seconds_per_round = 5;
        
        map = new Map(250, 20, scene, player);
        map->setPosition(0,0,0);
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/cactus_tall.obj");
        map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallA.obj");
        map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallB.obj");
        map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallC.obj");
        map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallD.obj");
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallE.obj");
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallF.obj");
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallG.obj");
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallH.obj");
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallI.obj");
        //map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/stone_tallJ.obj");
        map->addElementModel("data/kenney_natureKit_2.1/Models/OBJ format/statue_head.obj");

        map->populate();
        
        contact_listener = new ContactListener();
        scene->SetContactListener(contact_listener);
        
        camera->setTarget(&(player->transform));
        camera->setPitch(PI / 6);
        camera->setDistanceToTarget(10);
        
        seconds_passed = 0;
        restart_timer = 5;
    };
    
    void update(float dt){
        //async input to move the camera around
        if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_R)) Game::instance->setStage("test");
        //to navigate with the mouse fixed in the middle
        if (mouse_locked)
            Input::centerMouse();
        
        // update physics
        scene->Step();
        player->update(dt);
        aiManager->update(dt);
        map->update(dt);
        camera->update(dt);
        
        if (player->exploded){
            restart_timer -= dt;
            if (restart_timer <= 0){
                // change scene
                Game::instance->setStage("test");
            }
        }
        else
            seconds_passed += dt;
    };
    
    void render(){
        //set the clear color (the background color)
        glClearColor(0.0, 0.0, 0.0, 1.0);
        
        // Clear the window and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        //set the camera as default
        camera->enable();
         
         //set flags
         glDisable(GL_BLEND);
         glEnable(GL_DEPTH_TEST);
         glDisable(GL_CULL_FACE);
        
        if(shader)
        {
            //enable shader
            shader->enable();
            
            //upload uniforms
            shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
            
            //do the draw call
            player->render();
            aiManager->render();
            map->render();
            
            //disable shader
            shader->disable();
        }

        
         //Draw the floor grid
         drawGrid();
         
         //render the FPS, Draw Calls, etc
         drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
        
        std::stringstream stream;
        stream << "Survived for " << std::fixed << std::setprecision(2) << seconds_passed << " seconds";
        
        if (player->exploded){
            stream << std::endl << "Press [R] to restart..." << std::endl << "Restarting in " << (int) restart_timer;
            drawText(Game::instance->window_width / 4, Game::instance->window_height / 2, stream.str(), Vector3(0,0,0), 4);
        }
        else
            drawText(2, 20, stream.str(), Vector3(0,0,0), 2);

    };
    
    void deinit(){
        delete camera;
        delete player;
        delete aiManager;
        delete map;
        delete contact_listener;
        delete scene;
    };
};

class MainMenuStage : public Stage {
public:
    Camera* camera;
    
    void init() {
        
        //OpenGL flags
        //glEnable(GL_CULL_FACE); //render both sides of every triangle
        //glEnable(GL_DEPTH_TEST); //check the occlusions using the Z buffer

        //create our camera
        camera = new Camera();
        //camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0

        //camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
    }
    void update(float dt) {

    }

    void render() {
        //set the clear color (the background color)
        glClearColor(255.0, 255.0, 255.0, 1.0);
       
        // Clear the window and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //set the camera as default
        //camera->enable();

        if (renderButton(400, 100, 250, 100, Texture::Get("data/MainMenu/sprmenu1.png"), Texture::Get("data/MainMenu/sprmenu2.png"), false)) {
            Game::instance->setStage("test");
        }
        /*if (renderButton(400, 225, 250, 100, Texture::Get("data/MainMenu/sprload1.png"), Texture::Get("data/MainMenu/sprload2.png"), false)) { 
        }
        if (renderButton(400, 350, 250, 100, Texture::Get("data/MainMenu/sprops1.png"), Texture::Get("data/MainMenu/sprops2.png"), false)) {
        }*/
        if (renderButton(400, 225, 250, 100, Texture::Get("data/MainMenu/sprquit1.png"), Texture::Get("data/MainMenu/sprquit2.png"), false)) {
            Game::instance->must_exit = true;
        }
    }

    bool renderButton(float x, float y, float w, float h, Texture* tex, Texture* altTex, bool flipuvs) {
        
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Camera cam2D;
        cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);

        Mesh quad;
        //quad.createQuad(x, y, w + sin(Game::instance->time*16) * 10, h + sin(Game::instance->time*16) * 10, flipuvs);
        quad.createQuad(x, y, w, h, flipuvs);
        cam2D.enable();

        Vector2 mousePosition = Input::mouse_position;
        float halfWidth = w * 0.5;
        float halfHeith = h * 0.5;
        float min_x = x - halfWidth;
        float min_y = y - halfHeith;
        float max_x = x + halfWidth;
        float max_y = y + halfHeith;

        bool hoover = mousePosition.x > min_x && mousePosition.x < max_x && mousePosition.y > min_y && mousePosition.y < max_y;
        bool pressed = Input::isMousePressed(1);

        Shader* shader = Shader::Get("data/shaders/basic.vs","data/shaders/texture.fs");
        shader->enable();
        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
        shader->setUniform("u_texture", hoover ? altTex : tex, 0);

        Matrix44 quadModel;
        //quadModel.setTranslation(sin(Game::instance->time) * 10, 0, 0);
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        return hoover && pressed;
    }

    void deinit() {
        //delete camera;
    }
};

class IntroStage : public Stage {
public:
    void init() {

    }
    void update(float dt) {

    }
    void render() {
        renderUi(0, 0, Game::instance->window_width * 2, Game::instance->window_height * 2, Texture::Get("data/Intro/introScreenBackground.png"), false);
        renderUi(400, 250, 700, 70, Texture::Get("data/Intro/introScreenTittle.png"), false);
        drawText(300, 350, "Press SPACE to start!", Vector3(1, 1, 1), 2);
        if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) {
            Game::instance->setStage("mainMenu");
        }
    }

    void renderUi(float x, float y, float w, float h, Texture* tex, bool flipuvs) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Camera cam2D;
        cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);

        Mesh quad;
        //quad.createQuad(x, y, w + sin(Game::instance->time*16) * 10, h + sin(Game::instance->time*16) * 10, flipuvs);
        quad.createQuad(x, y, w, h, false);
        cam2D.enable();

        /*Vector2 mousePosition = Input::mouse_position;
        float halfWidth = w * 0.5;
        float halfHeith = h * 0.5;
        float min_x = x - halfWidth;
        float min_y = y - halfHeith;
        float max_x = x + halfWidth;
        float max_y = y + halfHeith;

        bool hoover = mousePosition.x > min_x && mousePosition.x < max_x&& mousePosition.y > min_y && mousePosition.y < max_y;
        bool pressed = Input::isMousePressed(1);*/

        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        shader->enable();
        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
        shader->setUniform("u_texture", tex, 0);

        Matrix44 quadModel;
        //quadModel.setTranslation(sin(Game::instance->time) * 10, 0, 0);
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
    void deinit() {

    }
};



#endif /* stages_h */

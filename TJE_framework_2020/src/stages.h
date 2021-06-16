//
//  stages.h
//  TJE_XCODE
//
//  Created by Laura Mac on 10/05/21.
//  Copyright © 2021 Laura Mac. All rights reserved.
//

#ifndef stages_h
#define stages_h

#include "stage.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "framework.h"
#include "q3.h"
#include "Vehicle.hpp"
#include "Map.hpp"
#include "PlayerCar.hpp"


class TestStage : public Stage {
public:
    bool mouse_locked = false;
    float speed = 2;
    Camera* camera;
    q3Scene* scene;
    PlayerCar* car;
    Map* map;
    Shader* shader;
    
    void init(){
        //OpenGL flags
        glEnable( GL_CULL_FACE ); //render both sides of every triangle
        glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
        
        //create our camera
        camera = new Camera();
        camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
        
        camera->setPerspective(70.f, Game::instance->window_width/(float)Game::instance->window_height,0.1f,10000.f); //set the projection, we want to be perspective
        
        //hide the cursor
        SDL_ShowCursor(!mouse_locked); //hide or show the mouse
        
        // create our physics world
        scene = new q3Scene(1.0/60.0, q3Vec3(0,-10,0));
        shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

       // car = VehicleFactory::createVehicle(scene);
        car = new PlayerCar(scene);
        car->setPosition(0,10,0);
        map = new Map(scene);
        map->setSize(1000);
        map->setPosition(0,0,0);
    };
    void update(float dt){
        //example
        
        //mouse input to rotate the cam
        if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
        {
            camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
            camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
        }
        
        //async input to move the camera around
        if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
        
        //to navigate with the mouse fixed in the middle
        if (mouse_locked)
            Input::centerMouse();
        
        // update physics
        scene->Step();
        car->update(dt);
        map->update(dt);
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
            car->render(shader);
            map->render(shader);
            
            //disable shader
            shader->disable();
        }

        
         //Draw the floor grid
         drawGrid();
         
         //render the FPS, Draw Calls, etc
         drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

    };
    
    void deinit(){
        delete camera;
    };
};


#endif /* stages_h */

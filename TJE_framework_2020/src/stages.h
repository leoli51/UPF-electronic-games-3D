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
#include "shader.h"
#include "texture.h"


class TestStage : public Stage {
public:
    bool mouse_locked = false;
    float speed = 10;
    Camera* camera;
    
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
        if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
        if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
        
        //to navigate with the mouse fixed in the middle
        if (mouse_locked)
            Input::centerMouse();
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
         
         //Draw the floor grid
         drawGrid();
         
         //render the FPS, Draw Calls, etc
         drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

    };
    
    void deinit(){
        delete camera;
    };
};

class MainMenuStage : public Stage {
public:

    Camera cam2D;

    void init() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
    }
    void update(float dt) {

    }

    void render() {
        //set the clear color (the background color)
        glClearColor(255.0, 255.0, 255.0, 1.0);
       
        // Clear the window and the depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderUi(0, 0, Game::instance->window_width * 2, Game::instance->window_height * 2, Texture::Get("data/Intro/introScreenBackground.png"), false);

        if (renderButton(400, 100, 250, 100, Texture::Get("data/MainMenu/sprmenu1.png"), Texture::Get("data/MainMenu/sprmenu2.png"), false)) {
            Game::instance->setStage("tutorial");
        }
        if (renderButton(400, 225, 250, 100, Texture::Get("data/MainMenu/sprquit1.png"), Texture::Get("data/MainMenu/sprquit2.png"), false)) {
            Game::instance->must_exit = true;
        }
    }

    bool renderButton(float x, float y, float w, float h, Texture* tex, Texture* altTex, bool flipuvs) {

        Mesh quad;
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
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();

        return hoover && pressed;
    }

    void renderUi(float x, float y, float w, float h, Texture* tex, bool flipuvs) {

        Mesh quad;
        quad.createQuad(x, y, w, h, false);
        cam2D.enable();

        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        shader->enable();
        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
        shader->setUniform("u_texture", tex, 0);

        Matrix44 quadModel;
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();
    }

    void deinit() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
};

class IntroStage : public Stage {
public:

    Camera cam2D;

    void init() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
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

        Mesh quad;
        quad.createQuad(x, y, w, h, false);
        cam2D.enable();

        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        shader->enable();
        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
        shader->setUniform("u_texture", tex, 0);

        Matrix44 quadModel;
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();
    }

    void deinit() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
};

class TutorialStage : public Stage {
public:

    Camera cam2D;

    void init() {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
    }
    void update(float dt) {

    }
    void render() {
        
        renderUi(0, 0, Game::instance->window_width * 2, Game::instance->window_height * 2, Texture::Get("data/Intro/introScreenBackground.png"), false);
        
        if (renderSimpleButton(600, 250, 260, 80, Texture::Get("data/Tutorial/playButton.png"), false)) {
            Game::instance->setStage("test");
        }

        drawText(100, 200, "How to play:", Vector3(1, 1, 1), 4);
        drawText(110, 250, "Press W to go forward", Vector3(1, 1, 1), 2);
        drawText(110, 300, "Press S to go backward", Vector3(1, 1, 1), 2);
        drawText(110, 350, "Press A to turn left", Vector3(1, 1, 1), 2);
        drawText(110, 400, "Press D to turn right", Vector3(1, 1, 1), 2);
        drawText(110, 450, "Press SPACE to handbrake", Vector3(1, 1, 1), 2);
        drawText(110, 500, "Press Down Arrow Key to toggle bigger view", Vector3(1, 1, 1), 2);
    }

    void renderUi(float x, float y, float w, float h, Texture* tex, bool flipuvs) {

        Mesh quad;
        quad.createQuad(x, y, w, h, false);
        cam2D.enable();

        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        shader->enable();
        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
        shader->setUniform("u_texture", tex, 0);

        Matrix44 quadModel;
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();
    }

    bool renderSimpleButton(float x, float y, float w, float h, Texture* tex, bool flipuvs) {

        Mesh quad;
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

        Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
        shader->enable();
        shader->setUniform("u_color", Vector4(1, 1, 1, 1));
        shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
        shader->setUniform("u_texture", tex, 0);

        Matrix44 quadModel;
        shader->setUniform("u_model", quadModel);
        shader->setUniform("u_texture_tiling", 1.0f);
        quad.render(GL_TRIANGLES);
        shader->disable();

        return hoover && pressed;
    }

    void deinit() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
};

#endif /* stages_h */

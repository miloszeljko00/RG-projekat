/**
 * @file main.cpp
 * @author Jovan Ivosevic
 * @brief Projections showcase
 * R - Changers rendered object
 * W, A, S, D - Rotates camera
 * Scroll - Changes scale
 * @version 0.1
 * @date 2022-10-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <functional>
#include <thread>
#include "ibufferable.hpp"
#include "irenderable.hpp"
#include "shader.hpp"
#include "tree.hpp"
#include "cube.hpp"
#include "buffer.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "ground.hpp"
#include "mountain.hpp"
#include "moon.hpp"
#include <random>

const int WindowWidth = 800;
const int WindowHeight = 800;
const std::string WindowTitle = "Projection";
const float TargetFPS = 60.0f;

float Clamp(float value, float min, float max) {
    return value < min ? min : value > max ? max : value;
}

struct Input {
    bool Left;
    bool Right;
    bool Up;
    bool Down;
    bool ChangeRenderable;
};

struct Renderer {
    unsigned mCurrRenderableIdx;
    IRenderable* mCurrRenderable;
    std::vector<IRenderable*> mRenderables;
    glm::vec2 mFramebufferSize;
    float mScalingFactor;
    bool IsDay;
};

struct EngineState {
    Camera* mCamera;
    Input* mInput;
    Renderer* mRenderer;
    float mDT;
    unsigned mCurrentFilter;
    float mInterpolation;
};

/**
 * @brief Error callback function for GLFW. See GLFW docs for details
 *
 * @param error Error code
 * @param description Error message
 */
static void ErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

/**
 * @brief Keyboard callback function for GLFW. See GLFW docs for details
 *
 * @param window GLFW window context object
 * @param key Triggered key GLFW code
 * @param scancode Triggered key scan code
 * @param action Triggered key action: pressed, released or repeated
 * @param mode Modifiers
 */
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    Input* UserInput = State->mInput;
    bool IsDown = action == GLFW_PRESS || action == GLFW_REPEAT || action == GLFW_KEY_DOWN;
    switch (key) {
    case GLFW_KEY_A: UserInput->Left = IsDown; break;
    case GLFW_KEY_D: UserInput->Right = IsDown; break;
    case GLFW_KEY_W: UserInput->Up = IsDown; break;
    case GLFW_KEY_S: UserInput->Down = IsDown; break;
    case GLFW_KEY_R: UserInput->ChangeRenderable = IsDown; break;
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    }
}

float lastX = 400, lastY = 300;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    Camera* Camera = State->mCamera;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera->mYaw += xoffset;
    Camera->mPitch += yoffset;

    if (Camera->mPitch > 89.0f)
        Camera->mPitch = 89.0f;
    if (Camera->mPitch < -89.0f)
        Camera->mPitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(Camera->mYaw)) * cos(glm::radians(Camera->mPitch));
    direction.y = sin(glm::radians(Camera->mPitch));
    direction.z = sin(glm::radians(Camera->mYaw)) * cos(glm::radians(Camera->mPitch));
    Camera->mCameraFront = glm::normalize(direction);
}

/**
 * @brief Scroll callback
 *
 * @param window GLFW window context object
 * @param xoffset Scroll offset on x-axis
 * @param yoffset Scroll offset on y-axis (used on most mouse wheels)
 */
static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    Camera* Camera = State->mCamera;
    Camera->mFOV -= (float)yoffset;
    if (Camera->mFOV < 1.0f)
        Camera->mFOV = 1.0f;
    if (Camera->mFOV > 90.0f)
        Camera->mFOV = 90.0f;
}

/**
 * @brief Framebuffer resize callback
 *
 * @param window GLFW window context object
 * @param width New framebuffer width
 * @param height New framebuffer height
 */
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    EngineState* State = (EngineState*)glfwGetWindowUserPointer(window);
    Renderer* Renderer = State->mRenderer;
    Renderer->mFramebufferSize = glm::vec2(width, height);
}

/**
 * @brief Updates engine state based on input
 *
 * @param state Engine State
 */
static void HandleInput(EngineState* state, float &currentFrame, float &lastFrame, float &deltaTime) {
    Input* UserInput = state->mInput;
    Camera* Camera = state->mCamera;
    Renderer* Renderer = state->mRenderer;
    float dt = state->mDT;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    Camera->mCameraSpeed = 2.5f * deltaTime;
    if (UserInput->Left) {
        Camera->mCameraPosition -= glm::normalize(glm::cross(Camera->mCameraFront, Camera->mCameraUp)) * Camera->mCameraSpeed;
        std::cout << "A pressed." << std::endl;
    }
    if (UserInput->Right) {
        Camera->mCameraPosition += glm::normalize(glm::cross(Camera->mCameraFront, Camera->mCameraUp)) * Camera->mCameraSpeed;
        std::cout << "D pressed." << std::endl;
    }
    if (UserInput->Down) {
        Camera->mCameraPosition -= Camera->mCameraSpeed * Camera->mCameraFront;
        std::cout << "S pressed." << std::endl;
    }
    if (UserInput->Up) {
        Camera->mCameraPosition += Camera->mCameraSpeed * Camera->mCameraFront;
        std::cout << "W pressed." << std::endl;
    }
    if (UserInput->ChangeRenderable) {
        Renderer->IsDay = !Renderer->IsDay;
        std::cout << "R pressed." << std::endl;
    }
}

void RenderSun(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Cube& SunBase, Cube& SunRotated);

void RenderGround(glm::mat4& ModelMatrix, Shader& BasicShader, Ground& Ground);

void RenderTree(glm::mat4& ModelMatrix, Shader& BasicShader, Tree& Tree, glm::vec3 Position);

void RenderMountain(glm::mat4& ModelMatrix, glm::vec3 Position, Shader& BasicShader, Mountain& Mountain, float scale);

void RenderCat(glm::mat4& ModelMatrix, Shader& BasicShader, Model& Cat);

void RenderMoon(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Moon& MoonBase, Moon& MoonRotated);


int main() {
    GLFWwindow* Window = 0;
    if (!glfwInit()) {
        std::cerr << "Failed to init glfw" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(ErrorCallback);
    Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle.c_str(), 0, 0);
    if (!Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    glfwSetKeyCallback(Window, KeyCallback);
    glfwSetCursorPosCallback(Window, mouse_callback);
    glfwSetScrollCallback(Window, ScrollCallback);
    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    GLenum GlewError = glewInit();
    if (GlewError != GLEW_OK) {
        std::cerr << "Failed to init glew: " << glewGetErrorString(GlewError) << std::endl;
        glfwTerminate();
        return -1;
    }

    Camera Camera(glm::vec3(6.5f, 4.0f, 4.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);

    Shader PhongShaderMaterialTexture("shaders/basic.vert", "shaders/basic.frag");

    float RenderDistance = 100.0f;
    glm::mat4 ModelMatrix(1.0f);
    glm::mat4 View = glm::lookAt(Camera.mCameraPosition, Camera.mTargetPosition, Camera.mWorldUp);

    Tree Tree;
    Cube SunBase;
    Cube SunRotated;
    Moon MoonBase;
    Moon MoonRotated;
    Ground Ground;
    Mountain Mountain;
    Model Cat("res/cat/cat.obj");
    if (!Cat.Load()) {
        std::cerr << "Failed to load model" << std::endl;
        glfwTerminate();
        return -1;
    }


    unsigned GrassTexture = Texture::LoadImageToTexture("res/grass/grass.jpg");
    unsigned GrassSpecularTexture = Texture::LoadImageToTexture("res/grass/specular.png");
    unsigned WoodTexture = Texture::LoadImageToTexture("res/wood/wood.png");
    unsigned SunTexture = Texture::LoadImageToTexture("res/sun/sun.png");
    unsigned MoonTexture = Texture::LoadImageToTexture("res/sun/moon.png");
    unsigned StoneTexture = Texture::LoadImageToTexture("res/stone/stone.jpg");
    unsigned CatTextureDiffuse = Texture::LoadImageToTexture("res/cat/cat_diffuse.jpg");
    unsigned CatTextureBump = Texture::LoadImageToTexture("res/cat/cat_bump.jpg");

    Renderer Renderer = { 0 };
    Renderer.mFramebufferSize = glm::vec2(WindowWidth, WindowHeight);
    Renderer.IsDay = true;
    EngineState State = { 0 };
    Input UserInput = { 0 };
    glfwSetWindowUserPointer(Window, &State);
    State.mCamera = &Camera;
    State.mInput = &UserInput;
    State.mRenderer = &Renderer;

    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();
    float TargetFrameTime = 1.0f / TargetFPS;
    glUseProgram(PhongShaderMaterialTexture.GetId());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /** NOTE(Jovan) : The main loop from a higher - level overview :
    * Retrieve inputs
    * Update state
    * Render result
    */
    float sc = 0;   // sun scale
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float Angle = 0;
    float Distance = 5.0f;
    const double lower_bound = 0.1;
    const double upper_bound = 0.5;
    double range = upper_bound - lower_bound;

    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(PhongShaderMaterialTexture.GetId());
        PhongShaderMaterialTexture.SetUniform1f("uPointLight11.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight11.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight11.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight12.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight12.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight12.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight13.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight13.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight13.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight14.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight14.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight14.Kq", 2.5f);



        PhongShaderMaterialTexture.SetUniform1f("uPointLight21.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight21.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight21.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight22.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight22.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight22.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight23.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight23.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight23.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight24.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight24.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight24.Kq", 2.5f);


        PhongShaderMaterialTexture.SetUniform1f("uPointLight31.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight31.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight31.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight32.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight32.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight32.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight33.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight33.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight33.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight34.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight34.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight34.Kq", 2.5f);



        PhongShaderMaterialTexture.SetUniform1f("uPointLight41.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight41.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight41.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight42.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight42.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight42.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight43.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight43.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight43.Kq", 2.5f);

        PhongShaderMaterialTexture.SetUniform1f("uPointLight44.Kc", 1.0f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight44.Kl", 2.5f);
        PhongShaderMaterialTexture.SetUniform1f("uPointLight44.Kq", 2.5f);



        PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Position", glm::vec3(2.1f, 2.0f, 1.0f));
        PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Direction", glm::vec3(-1.0f, -1.0f, -1.0f));
        PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Ka", glm::vec3(0.25f));
        PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Kd", glm::vec3(0.25f));
        PhongShaderMaterialTexture.SetUniform3f("uSpotlight.Ks", glm::vec3(0.25f));
        PhongShaderMaterialTexture.SetUniform1f("uSpotlight.Kc", 0.8f);
        PhongShaderMaterialTexture.SetUniform1f("uSpotlight.Kl", 0.092f);
        PhongShaderMaterialTexture.SetUniform1f("uSpotlight.Kq", 0.032f);
        PhongShaderMaterialTexture.SetUniform1f("uSpotlight.InnerCutOff", glm::cos(glm::radians(3.0f)));
        PhongShaderMaterialTexture.SetUniform1f("uSpotlight.OuterCutOff", glm::cos(glm::radians(3.0f)));
        
        if (Renderer.IsDay) {
            glClearColor(135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f);
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Direction", glm::vec3(-1.0f, -1.0f, -1.0f));
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Ka", glm::vec3(0.7f, 0.7f, 0.5f));
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Kd", glm::vec3(0.7f, 0.7f, 0.5f));
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Ks", glm::vec3(1.0f));
        }
        else {
            glClearColor(7.0f / 255, 0.0f / 255, 88.0f / 255, 1.0f);
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Direction", glm::vec3(-1.0f, -1.0f, -1.0f));
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Ka", glm::vec3(0.1f, 0.1f, 0.1f));
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Kd", glm::vec3(0.1f, 0.1f, 0.1f));
            PhongShaderMaterialTexture.SetUniform3f("uDirLight.Ks", glm::vec3(1.0f));
        }
        glfwPollEvents();
        glUseProgram(PhongShaderMaterialTexture.GetId());
        
        StartTime = glfwGetTime();
        HandleInput(&State, StartTime, lastFrame, deltaTime);
        View = glm::lookAt(Camera.mCameraPosition, Camera.mCameraPosition + Camera.mCameraFront, Camera.mCameraUp);
        PhongShaderMaterialTexture.SetView(View);

        //// NOTE(Jovan): Rotate point light around 0, 0, -2
        //glm::vec3 PointLightPosition(Distance * cos(Angle), 2.0f, -2.0f + Distance * sin(Angle));
        //PhongShaderMaterialTexture.SetUniform3f("uPointLight.Position", PointLightPosition);
        //Angle += State.mDT;
        

        // NOTE(Jovan): These calls are expensive and should be optimized by executing
        // them only when the framebuffer's size changes. This is for demo purposes only
        glm::mat4 Perspective = glm::perspective(glm::radians(Camera.mFOV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
        PhongShaderMaterialTexture.SetProjection(Perspective);
        
        // NOTE(Jovan): Diminishes the light's diffuse component by half, tinting it slightly red
        PhongShaderMaterialTexture.SetUniform1i("uMaterial.Kd", 0);
        // NOTE(Jovan): Makes the object really shiny
        PhongShaderMaterialTexture.SetUniform1i("uMaterial.Ks", 0);
        PhongShaderMaterialTexture.SetUniform1f("uMaterial.Shininess", 0.0f);
        if (Renderer.IsDay) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, SunTexture);
            RenderSun(sc, PhongShaderMaterialTexture, ModelMatrix, SunBase, SunRotated);
        }            
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MoonTexture);
            RenderMoon(sc, PhongShaderMaterialTexture, ModelMatrix, MoonBase, MoonRotated);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // NOTE(Jovan): Diminishes the light's diffuse component by half, tinting it slightly red
        PhongShaderMaterialTexture.SetUniform1i("uMaterial.Kd", 0);
        // NOTE(Jovan): Makes the object really shiny
        PhongShaderMaterialTexture.SetUniform1i("uMaterial.Ks", 1);
        PhongShaderMaterialTexture.SetUniform1f("uMaterial.Shininess", 128.0f);

        double x;
        double cos_x;
        glm::vec3 start_color;
        glm::vec3 end_color;
        glm::vec3 light_color;

        // point light 11
        PhongShaderMaterialTexture.SetUniform3f("uPointLight11.Position", glm::vec3(3.5f, 1.5f, 5.5f));
        x = glfwGetTime() * 2 + 51;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight11.Ka", light_color);
        
        x = glfwGetTime() * 6 + 11;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight11.Kd", light_color);

        x = glfwGetTime() * 12 + 521;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight11.Ks", light_color);


        

        // point light 12
        PhongShaderMaterialTexture.SetUniform3f("uPointLight12.Position", glm::vec3(2.5f, 1.5f, 5.5f));
        x = glfwGetTime() * 6 + 211;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 1.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight12.Ka", light_color);

        x = glfwGetTime() * 12 + 653;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight12.Kd", light_color);

        x = glfwGetTime() * 72 + 154;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight12.Ks", light_color);


        // point light 13
        PhongShaderMaterialTexture.SetUniform3f("uPointLight13.Position", glm::vec3(3.5f, 1.5f, 4.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 1.0f);
        end_color = glm::vec3(0.0f, 1.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight13.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight13.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight13.Ks", light_color);

        // point light 14
        PhongShaderMaterialTexture.SetUniform3f("uPointLight14.Position", glm::vec3(3.5f, 1.5f, 6.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(0.0f, 0.0f, 1.0f);
        end_color = glm::vec3(1.0f, 1.0f, 0.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight14.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight14.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight14.Ks", light_color);







        // point light 21
        PhongShaderMaterialTexture.SetUniform3f("uPointLight21.Position", glm::vec3(6.0f, 1.5f, -4.5f));
        x = glfwGetTime() * 2 + 51;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight21.Ka", light_color);

        x = glfwGetTime() * 6 + 11;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight21.Kd", light_color);

        x = glfwGetTime() * 12 + 521;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight21.Ks", light_color);




        // point light 22
        PhongShaderMaterialTexture.SetUniform3f("uPointLight22.Position", glm::vec3(4.0f, 1.5f, -4.5f));
        x = glfwGetTime() * 6 + 211;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 1.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight22.Ka", light_color);

        x = glfwGetTime() * 12 + 653;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight22.Kd", light_color);

        x = glfwGetTime() * 72 + 154;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight22.Ks", light_color);


        // point light 23
        PhongShaderMaterialTexture.SetUniform3f("uPointLight23.Position", glm::vec3(5.5f, 1.5f, -5.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 1.0f);
        end_color = glm::vec3(0.0f, 1.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight23.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight23.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight23.Ks", light_color);

        // point light 24
        PhongShaderMaterialTexture.SetUniform3f("uPointLight24.Position", glm::vec3(5.5f, 1.5f, -3.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(0.0f, 0.0f, 1.0f);
        end_color = glm::vec3(1.0f, 1.0f, 0.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight24.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight24.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight24.Ks", light_color);




        // point light 31
        PhongShaderMaterialTexture.SetUniform3f("uPointLight31.Position", glm::vec3(-8.0f, 1.5f, 4.5f));
        x = glfwGetTime() * 2 + 51;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight31.Ka", light_color);

        x = glfwGetTime() * 6 + 11;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight31.Kd", light_color);

        x = glfwGetTime() * 12 + 521;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight31.Ks", light_color);




        // point light 32
        PhongShaderMaterialTexture.SetUniform3f("uPointLight32.Position", glm::vec3(-6.0f, 1.5f, 4.5f));
        x = glfwGetTime() * 6 + 211;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 1.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight32.Ka", light_color);

        x = glfwGetTime() * 12 + 653;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight32.Kd", light_color);

        x = glfwGetTime() * 72 + 154;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight32.Ks", light_color);


        // point light 33
        PhongShaderMaterialTexture.SetUniform3f("uPointLight33.Position", glm::vec3(-7.5f, 1.5f, 5.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 1.0f);
        end_color = glm::vec3(0.0f, 1.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight33.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight33.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight33.Ks", light_color);

        // point light 34
        PhongShaderMaterialTexture.SetUniform3f("uPointLight34.Position", glm::vec3(-7.5f, 1.5f, 3.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(0.0f, 0.0f, 1.0f);
        end_color = glm::vec3(1.0f, 1.0f, 0.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight34.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight34.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight34.Ks", light_color);







        // point light 41
        PhongShaderMaterialTexture.SetUniform3f("uPointLight41.Position", glm::vec3(-5.0f, 1.5f, -6.5f));
        x = glfwGetTime() * 2 + 51;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight41.Ka", light_color);

        x = glfwGetTime() * 6 + 11;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight41.Kd", light_color);

        x = glfwGetTime() * 12 + 521;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight41.Ks", light_color);




        // point light 42
        PhongShaderMaterialTexture.SetUniform3f("uPointLight42.Position", glm::vec3(-3.0f, 1.5f, -6.5f));
        x = glfwGetTime() * 6 + 211;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 1.0f, 0.0f);
        end_color = glm::vec3(0.0f, 0.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight42.Ka", light_color);

        x = glfwGetTime() * 12 + 653;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight42.Kd", light_color);

        x = glfwGetTime() * 72 + 154;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight42.Ks", light_color);


        // point light 43
        PhongShaderMaterialTexture.SetUniform3f("uPointLight43.Position", glm::vec3(-4.5f, 1.5f, -7.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(1.0f, 0.0f, 1.0f);
        end_color = glm::vec3(0.0f, 1.0f, 1.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight43.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight43.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight43.Ks", light_color);

        // point light 44
        PhongShaderMaterialTexture.SetUniform3f("uPointLight44.Position", glm::vec3(-4.5f, 1.5f, -5.0f));
        x = glfwGetTime() * 6 + 24;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        start_color = glm::vec3(0.0f, 0.0f, 1.0f);
        end_color = glm::vec3(1.0f, 1.0f, 0.0f);
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight44.Ka", light_color);

        x = glfwGetTime() * 5 + 241;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight44.Kd", light_color);

        x = glfwGetTime() * 12 + 52;
        cos_x = cos(x);
        cos_x = (cos_x + 1.0) / 2.0;
        cos_x = lower_bound + cos_x * range;
        light_color = glm::mix(start_color, end_color, cos_x);
        PhongShaderMaterialTexture.SetUniform3f("uPointLight44.Ks", light_color);




        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GrassTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GrassSpecularTexture);
        RenderGround(ModelMatrix, PhongShaderMaterialTexture, Ground);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, WoodTexture);
        RenderTree(ModelMatrix, PhongShaderMaterialTexture, Tree, glm::vec3(3.0f, 0.0f, 5.0f));
        RenderTree(ModelMatrix, PhongShaderMaterialTexture, Tree, glm::vec3(5.0f, 0.0f, -4.0f));
        RenderTree(ModelMatrix, PhongShaderMaterialTexture, Tree, glm::vec3(-7.0f, 0.0f, 4.0f));
        RenderTree(ModelMatrix, PhongShaderMaterialTexture, Tree, glm::vec3(-4.0f, 0.0f, -6.0f));
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, StoneTexture);
        RenderMountain(ModelMatrix, glm::vec3(-10.0f, 0.0f, -30.0f), PhongShaderMaterialTexture, Mountain, 7);
        RenderMountain(ModelMatrix, glm::vec3(0.0f, 0.0f, -30.0f), PhongShaderMaterialTexture, Mountain, 5);
        RenderMountain(ModelMatrix, glm::vec3(10.0f, 0.0f, -30.0f), PhongShaderMaterialTexture, Mountain, 8);
        glBindTexture(GL_TEXTURE_2D, 0);




        // CAT
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CatTextureDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, CatTextureBump);
        RenderCat(ModelMatrix, PhongShaderMaterialTexture, Cat);
        glBindTexture(GL_TEXTURE_2D, 0);




        glUseProgram(0);
        glfwSwapBuffers(Window);
        UserInput = { 0 };
        EndTime = glfwGetTime();
        float WorkTime = EndTime - StartTime;
        if (WorkTime < TargetFrameTime) {
            int DeltaMS = (int)((TargetFrameTime - WorkTime) * 1000.0f);
            std::this_thread::sleep_for(std::chrono::milliseconds(DeltaMS));
            EndTime = glfwGetTime();
        }
        State.mDT = EndTime - StartTime;
    }
    glfwTerminate();
    return 0;
}

void RenderMoon(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Moon& MoonBase, Moon& MoonRotated)
{
    sc += 0.06;
    float scale1 = (abs(sin(sc)) + 2.5) / 3;
    float scale2 = (abs(cos(sc)) + 2.5) / 3;

    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 50.0f, -10.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale1));
    BasicShader.SetModel(ModelMatrix);
    MoonBase.Render();

    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 50.0f, -10.0f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale2));
    BasicShader.SetModel(ModelMatrix);
    MoonRotated.Render();
}


void RenderCat(glm::mat4& ModelMatrix, Shader& BasicShader, Model& Cat)
{
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.1f, 0.0f, 1.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.03));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    BasicShader.SetModel(ModelMatrix);
    Cat.Render();
}


void RenderMountain(glm::mat4& ModelMatrix, glm::vec3 Position, Shader& BasicShader, Mountain& Mountain, float scale)
{
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, Position);
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale));
    BasicShader.SetModel(ModelMatrix);
    Mountain.Render();
}

void RenderTree(glm::mat4& ModelMatrix, Shader& BasicShader, Tree& Tree, glm::vec3 Position)
{
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, Position);
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1));
    BasicShader.SetModel(ModelMatrix);
    Tree.Render();
}

void RenderGround(glm::mat4& ModelMatrix, Shader& BasicShader, Ground& Ground)
{
    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10));
    BasicShader.SetModel(ModelMatrix);
    Ground.Render();
}


void RenderSun(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Cube& SunBase, Cube& SunRotated)
{
    sc += 0.06;
    float scale1 = (abs(sin(sc)) + 2.5) / 3;
    float scale2 = (abs(cos(sc)) + 2.5) / 3;

    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 50.0f, -10.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale1));
    BasicShader.SetModel(ModelMatrix);
    SunBase.Render();

    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-60.0f, 50.0f, -10.0f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale2));
    BasicShader.SetModel(ModelMatrix);
    SunRotated.Render();
}
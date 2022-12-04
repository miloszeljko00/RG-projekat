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
};

struct EngineState {
    Camera* mCamera;
    Input* mInput;
    Renderer* mRenderer;
    float mDT;
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
        Renderer->mCurrRenderableIdx = ++Renderer->mCurrRenderableIdx % Renderer->mRenderables.size();
        Renderer->mCurrRenderable = Renderer->mRenderables[Renderer->mCurrRenderableIdx];
        UserInput->ChangeRenderable ^= true;
        std::cout << "R pressed." << std::endl;
    }
}

void RenderSun(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Cube& SunBase, Cube& SunRotatedX);

void RenderGround(glm::mat4& ModelMatrix, Shader& BasicShader, Ground& Ground);

void RenderTree(glm::mat4& ModelMatrix, Shader& BasicShader, Tree& Tree, glm::vec3 Position);

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

    Camera Camera(glm::vec3(-5.0f, 5.0f, 8.0f), glm::vec3(0.0f, 2.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.5f);

    Shader BasicShader("shaders/basic.vert", "shaders/basic.frag");
    float RenderDistance = 100.0f;
    glm::mat4 ModelMatrix(1.0f);
    glm::mat4 View = glm::lookAt(Camera.mCameraPosition, Camera.mTargetPosition, Camera.mWorldUp);

    Tree Tree;
    Cube SunBase;
    Cube SunRotated;
    Ground Ground;
    Model Fox("res/low-poly-fox/low-poly-fox.obj");
    if (!Fox.Load()) {
        std::cerr << "Failed to load model" << std::endl;
        glfwTerminate();
        return -1;
    }

    Model Alduin("res/alduin/alduin-dragon.obj");
    if (!Alduin.Load()) {
        std::cerr << "Failed to load model" << std::endl;
        glfwTerminate();
        return -1;
    }

    Model Amongus("res/amongus/amongus.obj");
    if (!Amongus.Load()) {
        std::cerr << "Failed to load model" << std::endl;
        glfwTerminate();
        return -1;
    }

    Model Spider("res/spider/spider.obj");
    if (!Spider.Load()) {
        std::cerr << "Failed to load model" << std::endl;
        glfwTerminate();
        return -1;
    } 

    Renderer Renderer = { 0 };
    Renderer.mFramebufferSize = glm::vec2(WindowWidth, WindowHeight);
    Renderer.mRenderables.push_back(&SunBase);
    Renderer.mRenderables.push_back(&SunRotated);
    Renderer.mRenderables.push_back(&Ground);
    Renderer.mRenderables.push_back(&Tree);
    Renderer.mRenderables.push_back(&Amongus);
    Renderer.mRenderables.push_back(&Fox);
    Renderer.mRenderables.push_back(&Alduin);
    Renderer.mRenderables.push_back(&Spider);
    Renderer.mCurrRenderable = Renderer.mRenderables[0];
    Renderer.mScalingFactor = 1.0f;

    EngineState State = { 0 };
    Input UserInput = { 0 };
    glfwSetWindowUserPointer(Window, &State);
    State.mCamera = &Camera;
    State.mInput = &UserInput;
    State.mRenderer = &Renderer;

    float StartTime = glfwGetTime();
    float EndTime = glfwGetTime();
    float TargetFrameTime = 1.0f / TargetFPS;

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    /** NOTE(Jovan) : The main loop from a higher - level overview :
    * Retrieve inputs
    * Update state
    * Render result
    */
    float sc = 0;   // sun scale
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f);
        glfwPollEvents();

        

        StartTime = glfwGetTime();
        HandleInput(&State, StartTime, lastFrame, deltaTime);
        glUseProgram(BasicShader.GetId());
        View = glm::lookAt(Camera.mCameraPosition, Camera.mCameraPosition + Camera.mCameraFront, Camera.mCameraUp);
        BasicShader.SetView(View);


        // NOTE(Jovan): These calls are expensive and should be optimized by executing
        // them only when the framebuffer's size changes. This is for demo purposes only
        glm::mat4 Perspective = glm::perspective(glm::radians(Camera.mFOV), (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
        BasicShader.SetProjection(Perspective);

        RenderSun(sc, BasicShader, ModelMatrix, SunBase, SunRotated);

        
        RenderGround(ModelMatrix, BasicShader, Ground);


        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(3.0f, 1.0f, 5.0f));
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(2.0f, 1.0f, -4.0f));
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(-5.0f, 1.0f, 2.0f));
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(-2.0f, 1.0f, -3.0f));


        glUseProgram(0);
        glfwSwapBuffers(Window);

        //NOTE(Jovan): Clear user input values after each frame
        UserInput = { 0 };

        // NOTE(Jovan): Time management (might not work :'c)
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
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -4.0f, -10.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(10));
    BasicShader.SetModel(ModelMatrix);
    Ground.Render();
}


void RenderSun(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Cube& SunBase, Cube& SunRotatedX)
{
    sc += 0.06;
    float scale1 = (abs(sin(sc)) + 2.5) / 3;
    float scale2 = (abs(cos(sc)) + 2.5) / 3;

    float c = (abs(sin(glfwGetTime())) + 2) / 3;
    unsigned int offsetLocation = glGetUniformLocation(BasicShader.mId, "offset");
    glUniform1f(offsetLocation, c);

    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.0f, 70.0f, -85.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale1));
    BasicShader.SetModel(ModelMatrix);
    SunBase.Render();

    ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(10.0f, 70.0f, -85.0f));
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(scale2));
    BasicShader.SetModel(ModelMatrix);
    SunRotatedX.Render();

    glUniform1f(offsetLocation, 1);
}




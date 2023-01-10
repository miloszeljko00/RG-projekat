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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

static unsigned LoadImageToTexture(const std::string& filePath);

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

    Shader BasicShader("shaders/basic.vert", "shaders/basic.frag");
    glUseProgram(BasicShader.GetId());
    BasicShader.SetUniform1i("uTexture1", 0);
    BasicShader.SetUniform1i("uTexture2", 1);
    /* NOTE(Jovan) : Set interpolation factor:
    * 0.0f - Uses first texture
    * 1.0f - Uses second
    * (0.0f, 1.0f) - Interpolates between the two
    */
    BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
    glUseProgram(0);

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


    unsigned GrassTexture = LoadImageToTexture("res/grass/grass.jpg");
    glBindTexture(GL_TEXTURE_2D, GrassTexture);
    // NOTE(Jovan): Generate MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);
    // NOTE(Jovan): Possible values: GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* NOTE(Jovan):
    * Possible mipmapping values for GL_TEXTURE_MIN_FILTER:
    *
    *  GL_NEAREST_MIPMAP_NEAREST
    *  GL_NEAREST_MIPMAP_LINEAR
    *  GL_LINEAR_MIPMAP_NEAREST
    *  GL_LINEAR_MIPMAP_LINEAR
    *
    * Two parts define the mipmapping algorithm
    * First - mipmap image picking
    *   NEAREST - Choose the mipmap that most closely matches the size of the pixel being textured
    *   LINEAR - Chooses two mipmaps that most closely match the size of the pixel being textured
    * Second - Color interpolation from picked image(s)
    *   NEAREST - Nearest neighbour algorithm
    *   LINEAR - Linear interpolation
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // NOTE(Jovan): Possible values: GL_NEAREST, GL_LINEAR. NO MIPMAPPING!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned WoodTexture = LoadImageToTexture("res/wood/wood.png");
    glBindTexture(GL_TEXTURE_2D, WoodTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned SunTexture = LoadImageToTexture("res/sun/sun.png");
    glBindTexture(GL_TEXTURE_2D, SunTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned MoonTexture = LoadImageToTexture("res/sun/moon.png");
    glBindTexture(GL_TEXTURE_2D, MoonTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned StoneTexture = LoadImageToTexture("res/stone/stone.jpg");
    glBindTexture(GL_TEXTURE_2D, StoneTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    unsigned CatTexture = LoadImageToTexture("res/cat/cat-diffuse.jpg");
    glBindTexture(GL_TEXTURE_2D, CatTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

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
    glUseProgram(BasicShader.GetId());

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
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(Renderer.IsDay)
            glClearColor(135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f);
        else
            glClearColor(7.0f / 255, 0.0f / 255, 88.0f / 255, 1.0f);
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
        
        if (Renderer.IsDay) {
            BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, SunTexture);
            RenderSun(sc, BasicShader, ModelMatrix, SunBase, SunRotated);
        }            
        else {
            BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MoonTexture);
            RenderMoon(sc, BasicShader, ModelMatrix, MoonBase, MoonRotated);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

            



        // NOTE(Jovan): Using only one texture, so setting interpolation to 0.0f
        BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, GrassTexture);
        RenderGround(ModelMatrix, BasicShader, Ground);
        glBindTexture(GL_TEXTURE_2D, 0);

        BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, WoodTexture);
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(3.0f, 0.0f, 5.0f));
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(5.0f, 0.0f, -4.0f));
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(-7.0f, 0.0f, 4.0f));
        RenderTree(ModelMatrix, BasicShader, Tree, glm::vec3(-4.0f, 0.0f, -6.0f));
        glBindTexture(GL_TEXTURE_2D, 0);

        BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, StoneTexture);
        RenderMountain(ModelMatrix, glm::vec3(-10.0f, 0.0f, -30.0f), BasicShader, Mountain, 7);
        RenderMountain(ModelMatrix, glm::vec3(0.0f, 0.0f, -30.0f), BasicShader, Mountain, 5);
        RenderMountain(ModelMatrix, glm::vec3(10.0f, 0.0f, -30.0f), BasicShader, Mountain, 8);
        glBindTexture(GL_TEXTURE_2D, 0);


        BasicShader.SetUniform1f("uInterpolationFactor", 0.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CatTexture);
        RenderCat(ModelMatrix, BasicShader, Cat);
        glBindTexture(GL_TEXTURE_2D, 0);


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

void RenderMoon(float& sc, Shader& BasicShader, glm::mat4& ModelMatrix, Moon& MoonBase, Moon& MoonRotated)
{
    sc += 0.06;
    float scale1 = (abs(sin(sc)) + 2.5) / 3;
    float scale2 = (abs(cos(sc)) + 2.5) / 3;

   /* float c = (abs(sin(glfwGetTime())) + 2) / 3;
    unsigned int offsetLocation = glGetUniformLocation(BasicShader.mId, "offset");
    glUniform1f(offsetLocation, c);*/

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

    //glUniform1f(offsetLocation, 1);
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

    /*float c = (abs(sin(glfwGetTime())) + 2) / 3;
    unsigned int offsetLocation = glGetUniformLocation(BasicShader.mId, "offset");
    glUniform1f(offsetLocation, c);*/

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

    //glUniform1f(offsetLocation, 1);
}

/**
 * @brief Loads image file and creates an OpenGL texture.
 * NOTE: Try avoiding .jpg and other lossy compression formats as
 * they are uncompressed during loading and the memory benefit is
 * negated with the addition of loss of quality
 *
 * @param filePath Image file path
 * @returns TextureID
 */
static unsigned
LoadImageToTexture(const std::string& filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath.c_str(), &TextureWidth, &TextureHeight, &TextureChannels, 0);
    // NOTE(Jovan): Images should usually flipped vertically as they are loaded "upside-down"
    stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

    // NOTE(Jovan): Checks or "guesses" the loaded image's format
    GLint InternalFormat = -1;
    switch (TextureChannels) {
    case 1: InternalFormat = GL_RED; break;
    case 3: InternalFormat = GL_RGB; break;
    case 4: InternalFormat = GL_RGBA; break;
    default: InternalFormat = GL_RGB; break;
    }

    unsigned Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
    glBindTexture(GL_TEXTURE_2D, 0);
    // NOTE(Jovan): ImageData is no longer necessary in RAM and can be deallocated
    stbi_image_free(ImageData);
    return Texture;
}



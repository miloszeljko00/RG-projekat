/**
 * @file camera.hpp
 * @author Milos Zeljko
 * @brief First Person Camera
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class Camera {
public:
    float mFOV;
    float mPitch;
    float mYaw;
    float mZoomSpeed;
    float mCameraSpeed;
    glm::vec3 mWorldUp;
    glm::vec3 mCameraPosition;
    glm::vec3 mTargetPosition;
    glm::vec3 mCameraDirection;
    glm::vec3 mCameraFront;
    glm::vec3 mCameraUp;
    glm::vec3 mCameraRight;

    /**
     * @brief Ctor
     *
     * @param fov - Vertical Field of View
     * @param distance - Camera distance from target
     * @param rotateSpeed - Camera rotation speed
     * @param zoomSpeed - Zoom speed
     * @param worldUp - World Up vector
     * @param target - Point in space the camera's looking at
     */
    Camera(glm::vec3 CameraPosition, glm::vec3 TargetPosition, glm::vec3 WorldUp, float CameraSpeed);

    /**
     * @brief Rotates the camera
     *
     * @param dYaw - How much the camera rotates around the Up vector. Delta Yaw
     * @param dPitch - How much the camera rotates around the Right vector. Delta Pitch
     * @param dt - Delta time
     */
    void Rotate(float dYaw, float dPitch, float dt);

    /**
     * @brief Moves the camera towards the target
     *
     * @param dy - How much the camera moves towards the target. Delta y
     * @param dt - Delta time
     */
    void Zoom(float dy, float dt);

private:

    /**
     * @brief Called after Rotating, Zooming, etc... Updates all the camera vectors accordingly
     *
     */
    void updateVectors();
};
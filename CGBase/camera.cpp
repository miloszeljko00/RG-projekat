#include "camera.hpp"


Camera::Camera(glm::vec3 CameraPosition, glm::vec3 TargetPosition, glm::vec3 WorldUp, float CameraSpeed) {
    mCameraPosition = CameraPosition;
    mTargetPosition = TargetPosition;
    mWorldUp = WorldUp;
    mCameraSpeed = CameraSpeed;

    mCameraDirection = glm::normalize(mCameraPosition - mTargetPosition);
    mCameraRight = glm::normalize(glm::cross(mWorldUp, mCameraDirection));
    mCameraUp = WorldUp;
    mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mFOV = 90;
}

void Camera::Rotate(float dYaw, float dPitch, float dt) {
    
}

void Camera::Zoom(float dy, float dt) {
}

void Camera::updateVectors() {
}
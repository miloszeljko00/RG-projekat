#include "cubebuffer.hpp"
#include "sunbuffer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::vector<float> SunBuffer::mVertices = {

};

SunBuffer::SunBuffer() {

    CubeBuffer* base = new CubeBuffer();
    CubeBuffer* rotatedX = new CubeBuffer();
    CubeBuffer* rotatedY = new CubeBuffer();
    CubeBuffer* rotatedZ = new CubeBuffer();

    glm::rotate

}

float* SunBuffer::GetVertices() {
    return mVertices.data();
}

unsigned SunBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned SunBuffer::GetVertexElementCount() {
    return 6;
}
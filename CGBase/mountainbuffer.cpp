#include "mountainbuffer.hpp"

const float r = 47.0f / 255.0f;
const float g = 79.0f / 255.0f;
const float b = 79.0f / 255.0f;

std::vector<float> MountainBuffer::mVertices = {
    
       // Donja stranica
       -1.0f, 0.0f, -1.0f,  r,g,b,
        1.0f, 0.0f, -1.0f,  r,g,b,
       -1.0f, 0.0f,  1.0f,  r,g,b,
        1.0f, 0.0f, -1.0f,  r,g,b,
        1.0f, 0.0f,  1.0f,  r,g,b,
       -1.0f, 0.0f,  1.0f,  r,g,b,


        1.0f, 0.0f, 1.0f,  r,g,b,
        1.0f, 0.0f, -1.0f,  r,g,b,
        0.0f, 5.0f, 0.0f,  r,g,b,

        0.0f, 5.0f, 0.0f,  r,g,b,
        1.0f, 0.0f, -1.0f,  r,g,b,
        -1.0f, 0.0f, -1.0f,  r,g,b,

        -1.0f, 0.0f, -1.0f,  r,g,b,
        -1.0f, 0.0f, 1.0f,  r,g,b,
        0.0f, 5.0f, 0.0f,  r,g,b,

        0.0f, 5.0f, 0.0f,  r,g,b,
        -1.0f, 0.0f, 1.0f,  r,g,b,
         1.0f, 0.0f, 1.0f,  r,g,b,


};

MountainBuffer::MountainBuffer() {}

float* MountainBuffer::GetVertices() {
    return mVertices.data();
}

unsigned MountainBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned MountainBuffer::GetVertexElementCount() {
    return 6;
}
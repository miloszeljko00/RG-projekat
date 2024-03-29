#include "mountainbuffer.hpp"


std::vector<float> MountainBuffer::mVertices = {
    
       // Donja stranica
       -1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
       -1.0f, 0.0f,  1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
       -1.0f, 0.0f,  1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
                           
                           
        1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 5.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                           
        0.0f, 5.0f,  0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
       -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                           
       -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
       -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.0f, 5.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                           
        0.0f, 5.0f,  0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 
       -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 
        1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 


};

MountainBuffer::MountainBuffer() {}

float* MountainBuffer::GetVertices() {
    return mVertices.data();
}

unsigned MountainBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned MountainBuffer::GetVertexElementCount() {
    return 8;
}
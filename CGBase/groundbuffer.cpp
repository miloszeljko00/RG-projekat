#include "groundbuffer.hpp"



std::vector<float> GroundBuffer::mVertices = {
    //      POS        |   Normale  | TEXTURE   
    //X     Y      Z   |  NX NY NZ  |  U  V


    //   podloga
   -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
   -10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f,  0.0f, 50.0f,
    10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f,
    10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f,
    10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 50.0f,  0.0f,
   -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
};

GroundBuffer::GroundBuffer() {}

float* GroundBuffer::GetVertices() {
    return mVertices.data();
}

unsigned GroundBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned GroundBuffer::GetVertexElementCount() {
    return 8;
}
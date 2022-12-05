#include "groundbuffer.hpp"

const float r = 33.0f / 255.0f;
const float g = 64.0f / 255.0f;
const float b =  1.0f / 255.0f;

std::vector<float> GroundBuffer::mVertices = {
    //      POS        |      COLOURS   
    //X     Y      Z      R     G     B


    //   podloga
   -10.0f, 0.0f, -10.0f,  r,g,b,
   -10.0f, 0.0f,  10.0f,  r,g,b,
    10.0f, 0.0f,  10.0f,  r,g,b,
    10.0f, 0.0f,  10.0f,  r,g,b,
    10.0f, 0.0f, -10.0f,  r,g,b,
   -10.0f, 0.0f, -10.0f,  r,g,b,

};

GroundBuffer::GroundBuffer() {}

float* GroundBuffer::GetVertices() {
    return mVertices.data();
}

unsigned GroundBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned GroundBuffer::GetVertexElementCount() {
    return 6;
}
#include "moonbuffer.hpp"

const float r = 254.0f / 255.0f;
const float g = 252.0f / 255.0f;
const float b = 215.0f / 255.0f;

std::vector<float> MoonBuffer::mVertices = {
    //      POS        |      COLOURS   
    //X     Y      Z      R     G     B


    //      Donja stranica kocke
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

   //      leva stranica kocke
  -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f,  -0.5f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,


  //      prednja stranica kocke
 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

  //      Gornja stranica kocke
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

   //      desna stranica kocke
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

   //      zadnja stranica kocke
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
};

MoonBuffer::MoonBuffer() {}

float* MoonBuffer::GetVertices() {
    return mVertices.data();
}

unsigned MoonBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned MoonBuffer::GetVertexElementCount() {
    return 8;
}
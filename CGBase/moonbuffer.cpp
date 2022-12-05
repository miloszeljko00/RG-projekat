#include "moonbuffer.hpp"

const float r = 254.0f / 255.0f;
const float g = 252.0f / 255.0f;
const float b = 215.0f / 255.0f;

std::vector<float> MoonBuffer::mVertices = {
    //      POS        |      COLOURS   
    //X     Y      Z      R     G     B


    //      Donja stranica kocke
   -0.5f, -0.5f, -0.5f,  r,g,b,
    0.5f, -0.5f, -0.5f,  r,g,b,
   -0.5f, -0.5f,  0.5f,  r,g,b,
    0.5f, -0.5f, -0.5f,  r,g,b,
    0.5f, -0.5f,  0.5f,  r,g,b,
   -0.5f, -0.5f,  0.5f,  r,g,b,

   //      leva stranica kocke
  -0.5f,  0.5f,  0.5f,  r,g,b,
  -0.5f,  0.5f, -0.5f,  r,g,b,
  -0.5f, -0.5f, -0.5f,  r,g,b,
  -0.5f, -0.5f,  -0.5f, r,g,b,
  -0.5f, -0.5f,  0.5f,  r,g,b,
  -0.5f,  0.5f,  0.5f,  r,g,b,


  //      prednja stranica kocke
 -0.5f,  0.5f,  0.5f,  r,g,b,
 -0.5f, -0.5f,  0.5f,  r,g,b,
  0.5f,  0.5f,  0.5f,  r,g,b,
 -0.5f, -0.5f,  0.5f,  r,g,b,
  0.5f, -0.5f,  0.5f,  r,g,b,
  0.5f,  0.5f,  0.5f,  r,g,b,

  //      Gornja stranica kocke
  -0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f,  0.5f, -0.5f,  r,g,b,
  -0.5f,  0.5f, -0.5f,  r,g,b,
  -0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f,  0.5f, -0.5f,  r,g,b,

   //      desna stranica kocke
   0.5f, -0.5f, -0.5f,  r,g,b,
   0.5f,  0.5f, -0.5f,  r,g,b,
   0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f, -0.5f,  0.5f,  r,g,b,
   0.5f, -0.5f, -0.5f,  r,g,b,

   //      zadnja stranica kocke
    0.5f,  0.5f, -0.5f,  r,g,b,
   -0.5f, -0.5f, -0.5f,  r,g,b,
   -0.5f,  0.5f, -0.5f,  r,g,b,
    0.5f,  0.5f, -0.5f,  r,g,b,
    0.5f, -0.5f, -0.5f,  r,g,b,
   -0.5f, -0.5f, -0.5f,  r,g,b,
};

MoonBuffer::MoonBuffer() {}

float* MoonBuffer::GetVertices() {
    return mVertices.data();
}

unsigned MoonBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned MoonBuffer::GetVertexElementCount() {
    return 6;
}
#include "groundbuffer.hpp"

const float r = 33.0f / 255.0f;
const float g = 64.0f / 255.0f;
const float b =  1.0f / 255.0f;

std::vector<float> GroundBuffer::mVertices = {
    //      POS        |      COLOURS   
    //X     Y      Z      R     G     B


    //      Donja stranica kocke
   -10.5f, -0.5f, -10.5f,  r,g,b,
    10.5f, -0.5f, -10.5f,  r,g,b,
   -10.5f, -0.5f,  10.5f,  r,g,b,
   -10.5f, -0.5f,  10.5f,  r,g,b,
    10.5f, -0.5f,  10.5f,  r,g,b,
    10.5f, -0.5f, -10.5f,  r,g,b,

    //      leva stranica kocke
   -10.5f, -0.5f, -10.5f,  r,g,b,
   -10.5f,  0.5f, -10.5f,  r,g,b,
   -10.5f,  0.5f,  10.5f,  r,g,b,
   -10.5f,  0.5f,  10.5f,  r,g,b,
   -10.5f, -0.5f,  10.5f,  r,g,b,
   -10.5f, -0.5f, -10.5f, r,g,b,


   //      prednja stranica kocke
  -10.5f,  0.5f,  10.5f,  r,g,b,
  -10.5f, -0.5f,  10.5f,  r,g,b,
   10.5f,  0.5f,  10.5f,  r,g,b,
   10.5f,  0.5f,  10.5f,  r,g,b,
   10.5f, -0.5f,  10.5f,  r,g,b,
  -10.5f, -0.5f,  10.5f,  r,g,b,

  //      Gornja stranica kocke
  -10.5f,  0.5f, -10.5f,  r,g,b,
   10.5f,  0.5f, -10.5f,  r,g,b,
  -10.5f,  0.5f,  10.5f,  r,g,b,
  -10.5f,  0.5f,  10.5f,  r,g,b,
   10.5f,  0.5f,  10.5f,  r,g,b,
   10.5f,  0.5f, -10.5f,  r,g,b,

   //      desna stranica kocke
   10.5f, -0.5f, -0.5f,  r,g,b,
   10.5f,  0.5f, -0.5f,  r,g,b,
   10.5f,  0.5f,  0.5f,  r,g,b,
   10.5f,  0.5f,  0.5f,  r,g,b,
   10.5f, -0.5f,  0.5f,  r,g,b,
   10.5f, -0.5f, -0.5f,  r,g,b,

   //      zadnja stranica kocke
   -10.5f,  0.5f, -0.5f,  r,g,b,
   -10.5f, -0.5f, -0.5f,  r,g,b,
    10.5f,  0.5f, -0.5f,  r,g,b,
    10.5f,  0.5f, -0.5f,  r,g,b,
    10.5f, -0.5f, -0.5f,  r,g,b,
   -10.5f, -0.5f, -0.5f,  r,g,b,
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
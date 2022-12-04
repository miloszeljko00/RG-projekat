#include "cubebuffer.hpp"

const float r = 255.0f / 255.0f;
const float g = 255.0f / 255.0f;
const float b = 0.0f / 255.0f;

std::vector<float> CubeBuffer::mVertices = {
    //      POS        |      COLOURS   
    //X     Y      Z      R     G     B


    //      Donja stranica kocke
   -0.5f, -0.5f, -0.5f,  r,g,b,
    0.5f, -0.5f, -0.5f,  r,g,b,
   -0.5f, -0.5f,  0.5f,  r,g,b,
   -0.5f, -0.5f,  0.5f,  r,g,b,
    0.5f, -0.5f,  0.5f,  r,g,b,
    0.5f, -0.5f, -0.5f,  r,g,b,

    //      leva stranica kocke
   -0.5f, -0.5f, -0.5f,  r,g,b,
   -0.5f,  0.5f, -0.5f,  r,g,b,
   -0.5f,  0.5f,  0.5f,  r,g,b,
   -0.5f,  0.5f,  0.5f,  r,g,b,
   -0.5f, -0.5f,  0.5f,  r,g,b,
   -0.5f, -0.5f,  -0.5f, r,g,b,


   //      prednja stranica kocke
  -0.5f,  0.5f,  0.5f,  r,g,b,
  -0.5f, -0.5f,  0.5f,  r,g,b,
   0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f,  0.5f,  0.5f,  r,g,b,
   0.5f, -0.5f,  0.5f,  r,g,b,
  -0.5f, -0.5f,  0.5f,  r,g,b,

  //      Gornja stranica kocke
  -0.5f,  0.5f, -0.5f,  r,g,b,
   0.5f,  0.5f, -0.5f,  r,g,b,
  -0.5f,  0.5f,  0.5f,  r,g,b,
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
   -0.5f,  0.5f, -0.5f,  r,g,b,
   -0.5f, -0.5f, -0.5f,  r,g,b,
    0.5f,  0.5f, -0.5f,  r,g,b,
    0.5f,  0.5f, -0.5f,  r,g,b,
    0.5f, -0.5f, -0.5f,  r,g,b,
   -0.5f, -0.5f, -0.5f,  r,g,b,
};

CubeBuffer::CubeBuffer() {}

float* CubeBuffer::GetVertices() {
    return mVertices.data();
}

unsigned CubeBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned CubeBuffer::GetVertexElementCount() {
    return 6;
}
#include "treebuffer.hpp"

std::vector<float> TreeBuffer::mVertices = {
        //      POS        |      COLOURS   
        //X     Y      Z      R     G     B
        


        //      Donja stranica stabla
        0.0f,  0.0f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.0f,  0.0f,  0.5f, 0.5f, 0.0f,
        0.0f,  0.0f,  0.0f,  0.0f, 0.5f, 0.0f,
        0.0f,  0.0f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.0f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.0f,  0.0f,  0.5f, 0.5f, 0.0f,

        // prednja stranica stabla
        0.0f,  0.0f,  0.0f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f,
        0.0f,  1.0f,  0.0f,  0.5f, 0.5f, 0.0f,
        0.0f,  0.0f,  0.0f,  0.5f, 0.0f, 0.0f,
        0.5f,  1.0f,  0.0f,  0.0f, 0.5f, 0.0f,

        // desna stranica stabla
        0.5f,  0.0f,  0.0f,  0.0f, 0.5f, 0.0f,
        0.5f,  0.0f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  1.0f, -0.5f,  0.5f, 0.5f, 0.0f,
        0.5f,  1.0f,  0.0f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.0f,  0.0f,  0.5f, 0.5f, 0.0f,
        0.5f,  1.0f, -0.5f,  0.0f, 0.5f, 0.0f,

        // zadnja stranica stabla
        0.5f,  0.0f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.0f,  0.0f, -0.5f,  0.5f, 0.5f, 0.0f,
        0.5f,  1.0f, -0.5f,  0.0f, 0.5f, 0.0f,
        0.5f,  1.0f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.0f,  0.0f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.0f,  1.0f, -0.5f,  0.5f, 0.5f, 0.0f,

        // leva stranica stabla
        0.0f,  1.0f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.0f,  0.0f, -0.5f,  0.0f, 0.5f, 0.0f,
        0.0f,  0.0f,  0.0f,  0.5f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,  0.5f, 0.5f, 0.0f,
        0.0f,  1.0f, -0.5f,  0.0f, 0.5f, 0.0f,
        0.0f,  0.0f,  0.0f,  0.5f, 0.0f, 0.0f,

        // gornja stranica stabla
        0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  1.0f,  0.0f,  0.5f, 0.5f, 0.0f,
        0.5f,  1.0f, -0.5f,  0.0f, 0.5f, 0.0f,
        0.5f,  1.0f, -0.5f,  0.0f, 0.0f, 0.0f,
        0.0f,  1.0f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,  0.5f, 0.5f, 0.0f,

        // donja stranica lisca
       -0.5f,  1.0f, -1.0f,  0.5f, 0.5f, 0.0f,
        1.0f,  1.0f, -1.0f,  1.0f, 0.5f, 0.0f,
        1.0f,  1.0f,  0.5f,  1.0f, 1.0f, 0.0f,
        1.0f,  1.0f,  0.5f,  0.5f, 0.5f, 0.0f,
       -0.5f,  1.0f,  0.5f,  1.0f, 0.5f, 0.0f,
       -0.5f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

       // leva stranica lisca
      -0.5f,  2.0f,  0.5f,  1.0f, 1.0f, 0.0f,
      -0.5f,  2.0f, -1.0f,  1.0f, 1.0f, 0.0f,
      -0.5f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
      -0.5f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
      -0.5f,  1.0f,  0.5f,  1.0f, 1.0f, 0.0f,
      -0.5f,  2.0f,  0.5f,  1.0f, 1.0f, 0.0f,


      // prednja stranica lisca
       1.0f,  1.0f,  0.5f,  0.5f, 0.5f, 0.0f,
       1.0f,  2.0f,  0.5f,  1.0f, 0.5f, 0.0f,
      -0.5f,  2.0f,  0.5f,  1.0f, 1.0f, 0.0f,
      -0.5f,  2.0f,  0.5f,  0.5f, 0.5f, 0.0f,
      -0.5f,  1.0f,  0.5f,  1.0f, 0.5f, 0.0f,
       1.0f,  1.0f,  0.5f,  1.0f, 1.0f, 0.0f,

      // desna stranica lisca
       1.0f,  1.0f, -1.0f,  0.5f, 0.5f, 0.0f,
       1.0f,  2.0f, -1.0f,  1.0f, 0.5f, 0.0f,
       1.0f,  2.0f,  0.5f,  1.0f, 1.0f, 0.0f,
       1.0f,  1.0f,  0.5f,  0.5f, 0.5f, 0.0f,
       1.0f,  1.0f, -1.0f,  1.0f, 0.5f, 0.0f,
       1.0f,  2.0f,  0.5f,  1.0f, 1.0f, 0.0f,

       // zadnja stranica lisca
       1.0f,  1.0f, -1.0f,  0.5f, 0.5f, 0.0f,
      -0.5f,  2.0f, -1.0f,  1.0f, 0.5f, 0.0f,
       1.0f,  2.0f, -1.0f,  1.0f, 1.0f, 0.0f,
       1.0f,  1.0f, -1.0f,  0.5f, 0.5f, 0.0f,
      -0.5f,  1.0f, -1.0f,  1.0f, 0.5f, 0.0f,
      -0.5f,  2.0f, -1.0f,  1.0f, 1.0f, 0.0f,
     
        
      // gornja stranica lisca
       1.0f,  2.0f,  0.5f,  0.5f, 0.5f, 0.0f,
       1.0f,  2.0f, -1.0f,  1.0f, 0.5f, 0.0f,
      -0.5f,  2.0f, -1.0f,  1.0f, 1.0f, 0.0f,
      -0.5f,  2.0f, -1.0f,  0.5f, 0.5f, 0.0f,
      -0.5f,  2.0f,  0.5f,  1.0f, 0.5f, 0.0f,
       1.0f,  2.0f,  0.5f,  1.0f, 1.0f, 0.0f,
};

TreeBuffer::TreeBuffer() {}

float* TreeBuffer::GetVertices() {
    return mVertices.data();
}

unsigned TreeBuffer::GetVertexCount() {
    return mVertices.size();
}

unsigned TreeBuffer::GetVertexElementCount() {
    return 6;
}
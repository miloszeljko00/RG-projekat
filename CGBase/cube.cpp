#include "cube.hpp"
#include "cubebuffer.hpp"

Cube::Cube() {
    CubeBuffer SunBuffer;
    mBuffer = new Buffer(SunBuffer);
}

void Cube::Render() {
    mBuffer->Render();
}
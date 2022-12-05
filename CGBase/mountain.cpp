#include "mountain.hpp"
#include "mountainbuffer.hpp"

Mountain::Mountain() {
    MountainBuffer MountainBuffer;
    mBuffer = new Buffer(MountainBuffer);
}

void Mountain::Render() {
    mBuffer->Render();
}
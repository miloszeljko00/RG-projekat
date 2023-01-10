#include "ground.hpp"
#include "groundbuffer.hpp"

Ground::Ground() {
    GroundBuffer GroundBuffer;
    mBuffer = new Buffer(GroundBuffer);
}

void Ground::Render() {
    mBuffer->Render();
}
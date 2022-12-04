#include "ground.hpp"
#include "groundbuffer.hpp"

Ground::Ground() {
    GroundBuffer SunBuffer;
    mBuffer = new Buffer(SunBuffer);
}

void Ground::Render() {
    mBuffer->Render();
}
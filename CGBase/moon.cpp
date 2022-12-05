#include "moon.hpp"
#include "moonbuffer.hpp"

Moon::Moon() {
    MoonBuffer MoonBuffer;
    mBuffer = new Buffer(MoonBuffer);
}

void Moon::Render() {
    mBuffer->Render();
}
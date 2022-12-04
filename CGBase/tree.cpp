#include "tree.hpp"
#include "treebuffer.hpp"

Tree::Tree() {
    TreeBuffer TreeBuffer;
    mBuffer = new Buffer(TreeBuffer);
}

void Tree::Render() {
    mBuffer->Render();
}
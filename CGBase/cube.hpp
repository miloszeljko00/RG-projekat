/**
 * @file tree.hpp
 * @author Milos Zeljko
 * @brief Tree
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <vector>

#include "irenderable.hpp"
#include "cubebuffer.hpp"
#include "buffer.hpp"

class Cube : public IRenderable {
public:

    /**
     * @brief Initializes cube buffer
     *
     */
    Cube();


    /**
     * @brief IRenderable Render implementation
     *
     */
    void Render();
private:
    Buffer* mBuffer;

};
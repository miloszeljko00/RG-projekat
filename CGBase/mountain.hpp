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
#include "mountainbuffer.hpp"
#include "buffer.hpp"

class Mountain : public IRenderable {
public:

    /**
     * @brief Initializes Mountain buffer
     *
     */
    Mountain();


    /**
     * @brief IRenderable Render implementation
     *
     */
    void Render();
private:
    Buffer* mBuffer;

};
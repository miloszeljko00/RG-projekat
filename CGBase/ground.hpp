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
#include "groundbuffer.hpp"
#include "buffer.hpp"

class Ground : public IRenderable {
public:
    Buffer* mBuffer;
    /**
     * @brief Initializes cube buffer
     *
     */
    Ground();


    /**
     * @brief IRenderable Render implementation
     *
     */
    void Render();
private:
    

};
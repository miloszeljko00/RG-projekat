/**
 * @file treebuffer.hpp
 * @author Milos Zeljko
 * @brief Tree buffer implementation
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <vector>
#include "ibufferable.hpp"

class MountainBuffer : public IBufferable {
public:
    /**
     * @brief Empty ctor
     *
     */
    MountainBuffer();
private:
    static std::vector<float> mVertices;

    virtual float* GetVertices();
    virtual unsigned GetVertexCount();
    virtual unsigned GetVertexElementCount();
};
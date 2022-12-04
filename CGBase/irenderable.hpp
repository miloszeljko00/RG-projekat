/**
 * @file irenderable.hpp
 * @author Milos Zeljko
 * @brief API for renderable objects
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

class IRenderable {
public:
    virtual void Render() = 0;
};
/**
 * @file ibufferable.hpp
 * @author Milos Zeljko
 * @brief API for bufferable objects
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
class IBufferable {
public:
	virtual float* GetVertices() = 0;
	virtual unsigned* GetIndices() {
		return 0;
	};
	virtual unsigned GetVertexCount() = 0;
	virtual unsigned GetIndexCount() {
		return 0;
	};
	virtual unsigned GetVertexElementCount() {
		return 6;
	};
};
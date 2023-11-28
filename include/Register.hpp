#pragma once
#include "Entity.hpp"
#include "Common.hpp"
#include <vector>

class Register {

	std::vector<Entity> entities;

public:

	Register() = default;
	void update();
	size_t addEntity();

	inline Entity* getEntityByID(ID id) {
		return &entities[id()];
	}


};
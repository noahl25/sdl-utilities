#include "Register.hpp"

void Register::update() {

	for (int x(0u); x < entities.size(); x++) {

		if (entities[x].isDead()) {

			entities.erase(entities.begin() + x);

		}

		for (Component* comp : entities[x].components) {
			comp->update();
		}
		
	}

}

size_t Register::addEntity() {

	entities.emplace_back(Entity());
	return entities.size() - 1;
}
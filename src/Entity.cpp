#include "Entity.hpp"


Entity::Entity() {
	id = getID();
	dead = false;
}

void Entity::kill() {
	dead = true;
}

bool Entity::isDead() {
	return dead;
}

uint8_t Entity::getID() {

	static int id = 0;
	if (id >= 255) {
		_ASSERT_FATAL("ENTITY OVERFLOW");
	}
	return id++;
}

Entity::~Entity() {

	for (Component* comp : components) {
		delete comp;
	}

}



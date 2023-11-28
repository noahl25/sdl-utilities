#pragma once

#include "Common.hpp"
#include "Vector2D.hpp"
#include "Debug.hpp"
#include "Components.hpp"

#include <vector>
#include <map>

class Register;

class Entity {

friend class Register;

private:

	bool dead;
	uint8_t id;
	std::vector<Component*> components;

	uint8_t getID();

	template <typename T> struct ComponentTrait {
	    static int getType() {
	        return T::type;
	    }
	};

public:

	Entity();
	~Entity();
	void kill();
	bool isDead();

	template <typename T, typename... TArgs> T* addComponent(TArgs&&... args) {
		//T temp
		//create component trait template class, but who really cares never mind i did it

		if (T::type != Component::Type::None) {

			for (auto* comp : components) {

				if ((comp->getType() & ComponentTrait<T>::getType()) > 0) { 

					return nullptr;

				}

			}
		}

		T* component = new T(std::move(args)...);
		component->id = components.size();
		components.push_back(component);
		return component;

	}

	template <typename T> T* getComponent() {

		for (Component* comp : components) {

			if (typeid(*comp) == typeid(T)) {
				T* comp = dynamic_cast<T*>(comp);
				if (comp) {
					return comp;
				}

				else {
					return nullptr;
				}
			}

		}

		return nullptr;

	}

	template <typename T> void deleteComponent() {

		for (uint8_t x(0u); x < components.size(); x++) {

			if (typeid(*components[x]) == typeid(T)) {

				components.erase(components.begin() + x);
				return;

			}

		}

	}



};


#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "primitive.hpp"
#include <../glm/glm/glm.hpp>

class Primitive;
class Cube;

class Light : public Cube {
public: 
	Light() {
		lightColor = glm::vec3(1.f,1.f, 1.f);
		lightPosition = glm::vec3(-3, 2, 0);
	}
	glm::vec3 lightColor;
	glm::vec3 lightPosition;
};


#endif
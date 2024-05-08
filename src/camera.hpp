#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <../glad/glad.h>
#include <iostream>


#include <../GLFW/glfw3.h>
#include <../glm/glm/glm.hpp>
#include <../glm/glm/gtx/transform.hpp>

using namespace glm;
class Camera {



public:
	float cameraSpeed;
	vec3 cameraTarget;
	vec3 cameraOrigin;
	vec3 up;
	vec3 front;
	vec3 cameraRight;
	mat4 view;
	vec3 direction;
	float yaw;
	float pitch;
	Camera() {}
	Camera(vec3 cameraOrigin, vec3 cameraTarget) {
		this->cameraTarget = cameraTarget;
		this->cameraOrigin = cameraOrigin;
		this->up = vec3(0.f, 1.f, 0.f);
		this->cameraRight = normalize(cross(up, cameraOrigin - cameraTarget));
		this->view = lookAt(cameraOrigin, cameraTarget, up);
		this->cameraSpeed = 0.5f;
		yaw = -90.f;
		pitch = 89.f;
		setDirection();
		front = normalize(direction);
	}
	mat4 getView() const {
		return view;
	}
	void setCameraTarget(vec3 value) {
		cameraTarget = value;
		view = lookAt(cameraOrigin, cameraTarget, up);
	}
	void setCameraOrigin(vec3 value) {
		cameraOrigin = value;
		view = lookAt(cameraOrigin, cameraTarget, up);
	}
	void setCameraSpeed(float value) {
		cameraSpeed = value;
	}

	void setDirection() {
		direction = vec3(cos(radians(yaw)) * cos(radians(pitch)), sin(radians(pitch)), sin(radians(yaw)) * cos(radians(pitch)));
		front = normalize(direction);
		view = lookAt(cameraOrigin, cameraTarget, up);
	}

	void setPitch(float pitch) {
		this->pitch = pitch;
		if (this->pitch > 89.f) {
			this->pitch = 89.f;
		}

		if (this->pitch < -89.f) {
			this->pitch = -89.f;
		}
		setDirection();
	}

	void setYaw(float yaw) {
		this->yaw += yaw;
		setDirection();
	}


	void moveCamera(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			setCameraOrigin(this->cameraOrigin + this->front * this->cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			setCameraOrigin(this->cameraOrigin - this->front * this->cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			setCameraOrigin(this->cameraOrigin - this->cameraRight * this->cameraSpeed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			setCameraOrigin(this->cameraOrigin + this->cameraRight * this->cameraSpeed);
		}
		std::cout << cameraOrigin[2] << std::endl;
	}
};


#endif
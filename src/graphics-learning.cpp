#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL
#define TINYOBJLOADER_IMPLEMENTATION


#include "camera.hpp"
#include "primitive.hpp"
#include "light.hpp"

#include <iostream>
#include <../glad/glad.h>
#include <../GLFW/glfw3.h>
#include <../util/shader.h>
#include <../util/stb_image.h>
#include <../glm/glm/glm.hpp>

using namespace glm;
using namespace std;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void getInput(GLFWwindow* window);
float fov = glm::radians(45.0f);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

Camera camera;
float lastX = 400, lastY = 300;
float sensitivity = 0.1f;
bool firstMouse = true;
int width = 800;
int height = 600;


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(width, height, "ChildishDev", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW Window" << endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Cube test = Cube();
	Sphere sphere = Sphere();
	Light cube = Light();
	Shader shader("C:\\Users\\baska\\source\\repos\\Lighting\\Lighting\\cubeshader.vert", "C:\\Users\\baska\\source\\repos\\Lighting\\Lighting\\cubeshader.frag");
	Shader lightShader("C:\\Users\\baska\\source\\repos\\Lighting\\Lighting\\lightshader.vert", "C:\\Users\\baska\\source\\repos\\Lighting\\Lighting\\lightshader.frag");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	camera = Camera(vec3(0, 0, 4), vec3(0, 0, 0));
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{

		shader.use();
		shader.setColor("lightPosition", cube.lightPosition.x, cube.lightPosition.y, cube.lightPosition.z, 1.0f);
		shader.setColor("lightColor", cube.lightColor.x, cube.lightColor.y, cube.lightColor.z, 1.0f);
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glBindVertexArray(VAO[0]);
		//glDrawArrays(GL_LINES, 0, 16);	


		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, vec3(0, 0, 0));
		glm::mat4 modelSphere = glm::mat4(0.5);
		modelSphere = glm::translate(modelSphere, vec3(3, 2, 0));
		//model = glm::rotate(model, radians(-55.0f), vec3(1, 0, 0));
		float deltaTime = 0.f;
		float lastFrame = 0.f;

		float timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;
		//camera.setCameraSpeed(2.5f * deltaTime);




		mat4 view = camera.getView();


		getInput(window);



		float aspectRatio = (float)width / (float)height; // width and height are the dimensions of your window
		float nearPlane = 0.1f; // near clipping plane
		float farPlane = 100.0f; // far clipping plane
		// field of view in radians

		glm::mat4 projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
		//projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		// update the uniform color

		mat4 mvp = projection * view * model;
		shader.setMatrix("MVP", (float*)&mvp);

		test.drawPrimitive();

		float transformation = sin(timeValue) / 2.0f + 0.5f;
		float transformationAngle = fmod(timeValue, 360.f) * glm::pi<float>() / 0.75f;
		mat4x4 transformationMatrix = glm::rotate(-transformationAngle, glm::vec3(1, 1, 1));
		modelSphere = transformationMatrix * modelSphere;
		mat4 mvpSphere = projection * view * modelSphere;
		shader.setMatrix("MVP", (float*)&mvpSphere);

		sphere.drawPrimitive();

		lightShader.use();
		glm::mat4 modelArea = glm::mat4(0.5);
		modelArea = glm::translate(modelArea, cube.lightPosition);
		modelArea = glm::scale(modelArea, vec3(0.1, 0.1, 0.1));
		mat4 mvpArea = projection * view * modelArea;
		lightShader.setMatrix("MVP", (float*)&mvpArea);
		lightShader.setColor("lightColor", cube.lightColor.x, cube.lightColor.y, cube.lightColor.z, 1.0f);
		cube.drawPrimitive();
		/*float greenValue = sin(timeValue) / 2.0f + 0.5f;
		float blueValue = cos(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, blueValue, 1.0f);*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	sphere.killPrimitive();
	test.killPrimitive();
	cube.killPrimitive();
	shader.kill();
	lightShader.kill();
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool fKeyPressed = false;

void getInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		// If the F key wasn't pressed in the previous frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		// If the F key wasn't pressed in the previous frame
		glfwSetWindowShouldClose(window, 1);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		// If the F key wasn't pressed in the previous frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.setCameraOrigin(camera.cameraOrigin + camera.front * camera.cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.setCameraOrigin(camera.cameraOrigin - camera.front * camera.cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.setCameraOrigin(camera.cameraOrigin - camera.cameraRight * camera.cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.setCameraOrigin(camera.cameraOrigin + camera.cameraRight * camera.cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		sensitivity -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		sensitivity += 0.01f;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	xOffset *= sensitivity;
	yOffset *= sensitivity;
	camera.setYaw(xOffset);
	camera.setPitch(camera.pitch + yOffset);
}
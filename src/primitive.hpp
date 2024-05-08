

#pragma once
#pragma message ("using double")

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <../glad/glad.h>
#include <../GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <../glm/glm/glm.hpp>
#include <../obj/tiny_obj_loader.h>
#include <cstring>


using namespace tinyobj;


typedef struct {
	std::vector<shape_t> shapes;
	attrib_t attributes;
	std::vector<material_t> materials;
} Object;

typedef struct {
	glm::vec3 vertices[3];
	glm::vec3 normals[3];
	glm::vec2 uvs[3];
} Triangle;
class Primitive {
	
public:
	Primitive() {}
	std::vector<glm::uvec3> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<float> drawData;
	std::vector<glm::uvec3> normalIndices;
	std::vector<Triangle> triangles;
	Object object; 
	unsigned int VBO, VAO, EBO;

	char path[100];
	const char* objectFile = "C:\\Users\\baska\\source\\repos\\Lighting\\Lighting\\";
	const char* indirect;
	
	//std::vector<glm::vec2> textureCoords;
	//std::vector<glm::vec3> vertexNormals;
	//glm::vec3 color;

	void drawPrimitive() { 
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, indices.size() * sizeof(glm::uvec3), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
	}

	void loadObj() {
		std::string error; 
		strcpy_s(path, objectFile);
		strcat_s(path, indirect);
		bool success = LoadObj(&object.attributes, &object.shapes, &object.materials, &error, path);
		if (!success) {
			std::cout << "ERROR OBJ LOADING: " << error << std::endl;
			return;
		}
		int test = sizeof(object.attributes.vertices);
		for (int i = 0; i <= object.attributes.vertices.size() - 3; i += 3) {
			auto& vertexDataObj = object.attributes.vertices;
			vertices.emplace_back(vertexDataObj[i], vertexDataObj[i+1], vertexDataObj[i+2]);
			
		}

		for (int i = 0; i <= object.attributes.texcoords.size() - 2; i += 2) {
			auto& vertexDataObj = object.attributes.texcoords;
			uvs.emplace_back(vertexDataObj[i], vertexDataObj[i + 1]);

		}
		for (int i = 0; i <= object.attributes.normals.size() - 3; i += 3) {
			auto& vertexDataObj = object.attributes.normals;
			normals.emplace_back(vertexDataObj[i], vertexDataObj[i + 1], vertexDataObj[i + 2]);
		
		}

		auto& shapeDataObj = object.shapes.at(0).mesh.indices;
		auto tester  = object.shapes.at(0).mesh.num_face_vertices.at(0);
		for (int i = 0; i <= shapeDataObj.size() - 3; i += 3) {
			indices.emplace_back(shapeDataObj[i].vertex_index, shapeDataObj[i + 1].vertex_index, shapeDataObj[i + 2].vertex_index);

            normalIndices.emplace_back(shapeDataObj[i].normal_index, shapeDataObj[i + 1].normal_index, shapeDataObj[i + 2].normal_index);
			Triangle triangle; 
			triangle.vertices[0] = vertices.at(shapeDataObj[i].vertex_index);
			triangle.vertices[1] = vertices.at(shapeDataObj[i + 1].vertex_index);
			triangle.vertices[2] = vertices.at(shapeDataObj[i + 2].vertex_index);
			triangle.normals[0] = normals.at(shapeDataObj[i].normal_index);
			triangle.normals[1] = normals.at(shapeDataObj[i+1].normal_index);
			triangle.normals[2] = normals.at(shapeDataObj[i+2].normal_index);
			triangle.uvs[0] = uvs.at(shapeDataObj[i].texcoord_index);
			triangle.uvs[1] = uvs.at(shapeDataObj[i+1].texcoord_index);
			triangle.uvs[2] = uvs.at(shapeDataObj[i+2].texcoord_index);

			triangles.emplace_back(triangle);
		}

	}

	void loadGpu() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		unsigned int NBO;
		glGenBuffers(1, &NBO);

		glBindVertexArray(VAO);
		for (int i = 0; i < triangles.size(); i++) {
			drawData.push_back(triangles[i].vertices[0].x);
            drawData.push_back(triangles[i].vertices[0].y);
            drawData.push_back(triangles[i].vertices[0].z);
            drawData.push_back(triangles[i].normals[0].x);
            drawData.push_back(triangles[i].normals[0].y);
            drawData.push_back(triangles[i].normals[0].z);
            drawData.push_back(triangles[i].uvs[0].x);
            drawData.push_back(triangles[i].uvs[0].y);
            drawData.push_back(triangles[i].vertices[1].x);
            drawData.push_back(triangles[i].vertices[1].y);
            drawData.push_back(triangles[i].vertices[1].z);
            drawData.push_back(triangles[i].normals[1].x);
            drawData.push_back(triangles[i].normals[1].y);
            drawData.push_back(triangles[i].normals[1].z);
            drawData.push_back(triangles[i].uvs[1].x);
            drawData.push_back(triangles[i].uvs[1].y);
            drawData.push_back(triangles[i].vertices[2].x);
            drawData.push_back(triangles[i].vertices[2].y);
            drawData.push_back(triangles[i].vertices[2].z);
            drawData.push_back(triangles[i].normals[2].x);
            drawData.push_back(triangles[i].normals[2].y);
            drawData.push_back(triangles[i].normals[2].z);
            drawData.push_back(triangles[i].uvs[2].x);
            drawData.push_back(triangles[i].uvs[2].y);
        }


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, drawData.size() * sizeof(float ), (void*)&drawData.front(), GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3), (void*)&indices.front(), GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void killPrimitive() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};


class Cube : public Primitive {
public:
	Cube();
};

class TriangularPrism : public Primitive {
public:
	TriangularPrism();
};

class Sphere : public Primitive {
public:
	Sphere();
};

class Line : public Primitive {
public: 
	Line();
};
#endif

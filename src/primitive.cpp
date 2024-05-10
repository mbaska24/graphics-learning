#include "primitive.hpp"




Cube::Cube() {
	indirect = "untitled.obj";
	loadObj();
	loadGpu();
}

TriangularPrism::TriangularPrism() {
	indirect = "sponza.obj";
	loadObj();
	loadGpu();
}

Sphere::Sphere() {
	indirect = "sphere.obj";
	loadObj();
	loadGpu();
}

Line::Line() {

	indirect = "area-light.obj";
	loadObj();
	loadGpu();
}

Torus::Torus() {
    indirect = "torus.obj";
    loadObj();
    loadGpu();
}
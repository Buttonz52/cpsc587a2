#pragma once
#include <glm\gtc\type_ptr.hpp>
#include <glm\glm.hpp>
#include <glad\glad.h>
#include <vector>

using namespace glm;
using namespace std;

class Particle
{
public:
	Particle(vec3 pos, bool anchored);

	bool isAnchored;
	vec3 force = vec3(0);
	vec3 velocity = vec3(0);
	vec3 position = vec3(0);
	float mass = 1.0;
	vector<vec3> verts;

	GLuint vertexBuffer;
	GLuint vertexArray;
	GLuint program;

	void generateBuffer();
	void render();
	void update();

private:

};


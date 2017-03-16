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
	Particle();
	Particle(vec3 pos);
	~Particle();

	vec3 force = vec3(0);
	vec3 velocity = vec3(0);
	vec3 position = vec3(0);
	float mass = 1.0;

	GLuint vertexArray;
	GLuint program;

	void generateBuffer(GLuint &vertexArray);
	void render(mat4 projection, mat4 view);
	void getCube(vector<vec3> verts, float r);

private:

};


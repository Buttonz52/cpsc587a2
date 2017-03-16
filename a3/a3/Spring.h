#include "Particle.h"
#pragma once
class Spring
{
public:
	Spring(Particle *a, Particle *b);
	~Spring();

	Particle *a;						//Particle a
	Particle *b;						//Particle b
	float len;						//Spring length
	float stiffness = 100.0;		//Spring Stiffness
	float dampening = 1.0;			//Spring Dampening	

	GLuint vertexArray;
	GLuint program;

	void generateBuffer(GLuint &vertexArray);
	void render(mat4 projection, mat4 view);

private:

};


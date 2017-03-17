#include "Particle.h"
#pragma once
class Spring
{
public:
	Spring(Particle *a, Particle *b);

	Particle *a;						//Particle a
	Particle *b;						//Particle b
	float len;						//Spring length
	float stiffness = 100.0;		//Spring Stiffness
	float dampening = 1.0;			//Spring Dampening	
	vector<vec3> verts;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint program;

	void generateBuffer();
	void render();
	void update();

private:

};


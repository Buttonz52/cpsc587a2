#include "Particle.h"
#pragma once
class Spring
{
public:
	Spring(Particle *a, Particle *b, float r_len);

	Particle *a;						//Particle a
	Particle *b;						//Particle b
	float len;							//Spring length
	float rest_len;						//Spring rest length
	float stiffness = 60.0;				//Spring Stiffness
	float dampening = 100.0;			//Spring Dampening	
	vector<vec3> verts;

	GLuint vertexArray;
	GLuint vertexBuffer;
	GLuint program;

	void generateBuffer();
	void render();
	void update();

private:

};


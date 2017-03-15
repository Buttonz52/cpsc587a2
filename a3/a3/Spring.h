#include "Particle.h"
#pragma once
class Spring
{
public:
	Spring();
	~Spring();

	Particle a;						//Particle a
	Particle b;						//Particle b
	float len;						//Spring length
	float stiffness = 100.0;		//Spring Stiffness
	float dampening = 1.0;			//Spring Dampening	

private:

};


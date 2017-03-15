#include "Particle.h"
#include "Shader.h"


Particle::Particle()
{

}

Particle::Particle(vec3 pos)
{
	position = pos;
	vertexArray = 0;
	program = generateProgram("general.vert", "general.frag");
	generateBuffer(vertexArray);
}

Particle::~Particle()
{
}

void Particle::getCube(vector<vec3> verts, float r)
{
	verts.push_back(vec3(-r, r, r));	//1
	verts.push_back(vec3( r, r, r));	//2
	verts.push_back(vec3(-r,-r, r));	//3
	verts.push_back(vec3(-r,-r, r));	//4
	verts.push_back(vec3(-r,-r,-r));	//5
	verts.push_back(vec3( r,-r,-r));	//6
	verts.push_back(vec3(-r, r,-r));	//7
	verts.push_back(vec3( r, r,-r));	//8
	verts.push_back(vec3(-r, r, r));	//9
	verts.push_back(vec3( r, r, r));	//10
	verts.push_back(vec3( r,-r, r));	//11
	verts.push_back(vec3( r, r,-r));	//12
	verts.push_back(vec3( r,-r,-r));	//13
	verts.push_back(vec3(-r,-r,-r));	//14
	verts.push_back(vec3(-r, r,-r));	//15
	verts.push_back(vec3(-r,-r, r));	//16
	verts.push_back(vec3(-r, r, r));	//17
}

void Particle::generateBuffer(GLuint &vertexArray)
{
	GLuint vertexBuffer = 0;

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	vector<vec3> verts;
	float r = 0.5;

	verts.push_back(vec3(-r, r, r));	//1
	verts.push_back(vec3(r, r, r));	//2
	verts.push_back(vec3(-r, -r, r));	//3
	verts.push_back(vec3(-r, -r, r));	//4
	verts.push_back(vec3(-r, -r, -r));	//5
	verts.push_back(vec3(r, -r, -r));	//6
	verts.push_back(vec3(-r, r, -r));	//7
	verts.push_back(vec3(r, r, -r));	//8
	verts.push_back(vec3(-r, r, r));	//9
	verts.push_back(vec3(r, r, r));	//10
	verts.push_back(vec3(r, -r, r));	//11
	verts.push_back(vec3(r, r, -r));	//12
	verts.push_back(vec3(r, -r, -r));	//13
	verts.push_back(vec3(-r, -r, -r));	//14
	verts.push_back(vec3(-r, r, -r));	//15
	verts.push_back(vec3(-r, -r, r));	//16
	verts.push_back(vec3(-r, r, r));	//17

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Particle::render(mat4 projection, mat4 view)
{
	glBindVertexArray(vertexArray);
	glUseProgram(program);

	//vec4 pos = projection * modelview * vec4(position,1);
	mat4 model = mat4(1);
	model[3] = vec4(position, 1.0);
	mat4 MVP = projection * view * model;

	//glUniform4fv(glGetUniformLocation(program, "position"), 1, value_ptr(MVP));
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, value_ptr(MVP));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}
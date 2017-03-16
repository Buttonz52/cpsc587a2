#include "Spring.h"
#include "Shader.h"



Spring::Spring(Particle *_a, Particle *_b)
{
	a = _a;
	b = _b;
	len = (b->position - a->position).length();	
	vertexArray = 0;
	program = generateProgram("general.vert", "general.frag");
	generateBuffer(vertexArray);
}


Spring::~Spring()
{
}

void Spring::generateBuffer(GLuint &vertexArray)
{
	GLuint vertexBuffer = 0;

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	vector<vec3> verts;

	verts.push_back(a->position);
	verts.push_back(b->position);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Spring::render(mat4 projection, mat4 view)
{
	glBindVertexArray(vertexArray);
	glUseProgram(program);

	mat4 model = mat4(1);
	//model[3] = vec4(a->position, 1.0);
	mat4 MVP = projection * view * model;

	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, value_ptr(MVP));

	glDrawArrays(GL_LINE, 0, 2);

	glBindVertexArray(0);
}

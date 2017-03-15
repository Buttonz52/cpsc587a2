#version 430 core

layout (location = 0) in vec3 vertex;

uniform mat4 MVP;
//uniform vec4 position;

out vec3 vert;

void main (void)
{
    gl_Position = MVP * vec4(vertex, 1.f);
	//gl_Position = position;
}

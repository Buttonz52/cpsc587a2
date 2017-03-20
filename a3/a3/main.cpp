#include "main.h"

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 32);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CPSC587 Assignment 3", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	printOpenGLVersion();

	setupScene();

	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3, 0.3, 0.3, 1.0);

		//update objects
		simulate();

		//update 
		for (int i = 0; i < particles.size(); i++)
			particles[i]->update();
		for (int i = 0; i < springs.size(); i++)
			springs[i]->update();

		//draw objects
		for (int i = 0; i < particles.size(); i++)
			particles[i]->render();

		for (int i = 0; i < springs.size(); i++)
			springs[i]->render();

		//time step
		curr_t += delta_t;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Shutdow the program
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void setupScene()
{
	Particle* p1 = new Particle(vec3(0,0.5,0), true);	
	Particle* p2 = new Particle(vec3(0,-0.5,0), false);	
	particles.push_back(p1);
	particles.push_back(p2);

	Spring* s1 = new Spring(p1,p2);
	springs.push_back(s1);
}

void simulate()
{
	if (particles.size() < 2)
		cout << "There are no springs in the spring sytem";

	//for each spring update the forces for each particle
	for (int i = 0; i < springs.size(); i++)
	{
		float k = springs[i]->stiffness;
		float b = springs[i]->dampening;
		vec3 xi = springs[i]->a->position;
		vec3 xi1 = springs[i]->b->position;
		vec3 vi = springs[i]->a->velocity;
		vec3 vi1 = springs[i]->b->velocity;
		vec3 f = -k * (xi - xi1) - b * (vi - vi1);

		springs[i]->a->force = f;
		springs[i]->b->force = -f;
	}
		
	//for each particle move the particle appropriatly
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->isAnchored)
		{
			particles[i]->velocity += delta_t*(particles[i]->force) / (particles[i]->mass);
			particles[i]->position += particles[i]->velocity;
			particles[i]->force = vec3(0, 0, 0);
		}
	}	
	curr_t += delta_t;
}

void printOpenGLVersion()
{
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("OpenGL on %s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
	printf("GLSL version supported %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("GL version major, minor: %i.%i\n", major, minor);
}

void errorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR " << error << ": " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

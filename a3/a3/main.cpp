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
		glClearBufferfv(GL_COLOR, 0, clearColor);

		//update objects
		simulate(springs, particles);

		//update 


		//draw objects
		for(int i = 0; i < particles.size(); i++)
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
	Particle* p1 = new Particle(vec3(0,0,0));	
	Particle* p2 = new Particle(vec3(0,-0.1,0));	
	particles.push_back(p1);
	particles.push_back(p2);

	Spring* s1 = new Spring(p1,p2);
	springs.push_back(s1);
}

void simulate(vector<Spring*> s, vector<Particle*> p)
{
	if (p.size() < 2)
		cout << "There are no springs in the spring sytem";

	//for each spring update the forces for each particle
	//for each particle move the particle appropriatly
	
	
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

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

		//update spring/mass system
		simulate();

		//update vertex positions
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

		//v-sync
		glfwSwapInterval(1);

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
	if (scene == 1)
	{
		Particle* p1 = new Particle(vec3(0, 0, 0), 1.0, true);
		Particle* p2 = new Particle(vec3(0, -0.01, 0), 1.0, false);
		particles.push_back(p1);
		particles.push_back(p2);

		Spring* s1 = new Spring(p1, p2, 0.1);
		springs.push_back(s1);
	}
	if (scene == 2)
	{
		Particle* p1 = new Particle(vec3(0, 0.5, 0), 0.5, true);
		Particle* p2 = new Particle(vec3(0.2, 0.5, 0), 0.5, false);
		Particle* p3 = new Particle(vec3(0.4, 0.5, 0), 0.5, false);
		particles.push_back(p1);
		particles.push_back(p2);
		particles.push_back(p3);

		Spring* s1 = new Spring(p1, p2, 0.1);
		Spring* s2 = new Spring(p2, p3, 0.1);
		springs.push_back(s1);
		springs.push_back(s2);
	}
	if (scene == 3)
	{
		for (int i = 0; i < cubeSize; i++)
		{
			for (int j = 0; j < cubeSize; j++)
			{
				for (int k = 0; k < cubeSize; k++)
				{
					Particle* p = new Particle(vec3(i*d, j*d, k*d), 0.5, true);
					particles.push_back(p);
				}
			}
		}

		connectSprings3();

	}
	if (scene == 4)
	{

	}
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
		float x = length(springs[i]->b->position - springs[i]->a->position);
		float xr = springs[i]->rest_len;
		vec3 dir = -normalize(springs[i]->b->position - springs[i]->a->position);
		vec3 vi = springs[i]->a->velocity;
		vec3 vi1 = springs[i]->b->velocity;
		vec3 f = -k * (x - xr)*dir - b * (vi - vi1);			//spring force and dampening force

		springs[i]->a->force += f;
		springs[i]->b->force += -f;
	}
		
	//for each particle move the particle appropriatly and remove its force
	for (int i = 0; i < particles.size(); i++)
	{
		if (!particles[i]->isAnchored)
		{
			particles[i]->force += GRAVITY*particles[i]->mass;			//force of gravity
			particles[i]->velocity += delta_t*(particles[i]->force) / (particles[i]->mass);
			particles[i]->position += particles[i]->velocity;
			particles[i]->force = vec3(0, 0, 0);
		}
		else {
			particles[i]->force = vec3(0, 0, 0);
		}
	}	
	curr_t += delta_t;
}

void connectSprings3()
{
	float c = sqrt(d*d + d*d);
	float r = sqrt(d*d + c*c);

	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = 0; j < particles.size(); j++)
		{
			Particle* x1 = particles[i];
			Particle* x2 = particles[j];

			//continue here ----------------------------------------------------------------------------------
		}
	}
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
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		scene = 1;
		particles.clear();
		springs.clear();
		setupScene();
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		scene = 2;
		particles.clear();
		springs.clear();
		setupScene();
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		scene = 3;
		particles.clear();
		springs.clear();
		setupScene();
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		scene = 4;
		particles.clear();
		springs.clear();
		setupScene();
	}
}

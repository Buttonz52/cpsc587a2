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
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CPSC587 Assignment 3", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetCursorPosCallback(window, motion);
	glfwSetScrollCallback(window, scroll);
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
		for (int i = 0; i < 10; i++)
			simulate();

		//update vertex positions
		for (int i = 0; i < particles.size(); i++)
			particles[i]->update();
		for (int i = 0; i < springs.size(); i++)
			springs[i]->update();

		//draw objects
		//for (int i = 0; i < particles.size(); i++)
			//particles[i]->render();

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

		Spring* s1 = new Spring(p1, p2, 0.1, 40, 20);
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

		Spring* s1 = new Spring(p1, p2, 0.15, 40, 20);
		Spring* s2 = new Spring(p2, p3, 0.15, 40, 20);
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
					Particle* p = new Particle(vec3(i*d1, j*d1, k*d1), 1, false);
					particles.push_back(p);
				}
			}
		}
		connectSprings3();
	}
	if (scene == 4)
	{
		for (int i = 0; i < cloth_x; i++)
		{
			for (int j = 0; j < cloth_y; j++)
			{
				Particle* p = new Particle(vec3(i*d2 - 0.2, 0.8, j*d2 + 0.3), 1, false);
				particles.push_back(p);
			}
		}
		particles[0]->isAnchored = true;
		
		connectSprings4();
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
		vec3 fs = -k * (x - xr)*dir;
		vec3 fd;
		
		if (length(fs) > 0)
		{
			vec3 fsn = normalize(fs);
			fd = -b * dot(vi - vi1, fsn)*fsn;
		}

		vec3 f = fs + fd;			//spring force and dampening force

		springs[i]->a->force += f;
		springs[i]->b->force += -f;
	}
		
	//for each particle move the particle appropriatly and remove its force
	for (int i = 0; i < particles.size(); i++)
	{
		if (!particles[i]->isAnchored)
		{
			if (particles[i]->position.y > -0.95)
			{
				particles[i]->force += GRAVITY*particles[i]->mass;			//force of gravity
				if(scene == 4)
					particles[i]->force += -particles[i]->velocity*500.f;
				else
					particles[i]->force += -particles[i]->velocity*50.f;
				particles[i]->velocity += delta_t*(particles[i]->force) / (particles[i]->mass);
				particles[i]->position += particles[i]->velocity;
			}

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
	float c = sqrt(d1*d1 + d1*d1);
	float r = sqrt(d1*d1 + c*c);

	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = i; j < particles.size(); j++)
		{
			Particle* x1 = particles[i];
			Particle* x2 = particles[j];
			float len = length(vec3(x2->position - x1->position));

			if (!(x1 == x2) && len < r)
			{
				Spring* s = new Spring(x1, x2, len, 1000, 100);
				springs.push_back(s);
			}
		}
	}
}
void connectSprings4()
{
	float c = sqrt(d2*d2 + d2*d2);
	float r = sqrt(d2*d2 + c*c);

	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = i; j < particles.size(); j++)
		{
			Particle* x1 = particles[i];
			Particle* x2 = particles[j];
			float len = length(vec3(x2->position - x1->position));

			if (!(x1 == x2) && len < r)
			{
				Spring* s = new Spring(x1, x2, len, 12000, 2000);
				springs.push_back(s);
			}
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

void mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouse_old_x = x;
		mouse_old_y = y;
	}
}

void motion(GLFWwindow* w, double x, double y)
{

	double dx, dy;
	dx = (x - mouse_old_x);
	dy = (y - mouse_old_y);

	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_1))
	{
		rotate_x += dy * 0.005f;
		phi += dx * 0.005f;
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

void scroll(GLFWwindow* w, double x, double y)
{
	double dy;
	dy = (x - y);
	s += dy * 0.03f;
	if (s < 0.03)
		s = 0.03;
}
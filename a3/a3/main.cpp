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
	glfwSetScrollCallback(window, scroll);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetCursorPosCallback(window, motion);
	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	printOpenGLVersion();

	setupScene();

	camera.setInitValues();
	mat4 projection = camera.calculateProjectionMatrix();
	mat4 view = camera.calculateViewMatrix();

	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, clearColor);

		//update objects
		simulate(springs, particles, curr_t, delta_t);

		//update camera
		projection = camera.calculateProjectionMatrix();
		view = camera.calculateViewMatrix();

		//draw objects
		//for(int i = 0; i < particles.size(); i++)
			particles[0]->render(projection, view);

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
	Particle* p1 = new Particle(vec3(0,1,0));	
	Particle* p2 = new Particle(vec3(0,0,0));	
	particles.push_back(p1);
	particles.push_back(p2);

	Spring* s1 = new Spring();
	springs.push_back(s1);
	connectSprings(springs, particles);
}

void simulate(vector<Spring*> s, vector<Particle*> p, float curr_t, float delta_t)
{
	if (p.size() < 2)
		cout << "There are no springs in the spring sytem";

	//change position for each s[i] and p[i]
	p[0]->position -= vec3(0, -0.1, 0);
	
	
}

void connectSprings(vector<Spring*> s, vector<Particle*> p)
{
	for (int i = 0; i < s.size(); i++)
	{
		s[i]->a = *p[i];
		s[i]->b = *p[i + 1];
		s[i]->len = (s[i]->b.position - s[i]->a.position).length();	//fix me
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

void scroll(GLFWwindow* window, double x, double y)
{
	camera.incrementRadius(y * 2.0);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
	int vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	glViewport(0, 0, width, height);

	float minDim = float(min(width, height));

	winRatio[0][0] = minDim / float(width);

	camera.setAsp((float)width / (float)height);
}

void motion(GLFWwindow* w, double x, double y)
{

	double dx, dy;
	dx = (x - mouse_old_x);
	dy = (y - mouse_old_y);

	if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_1))
	{
		camera.incrementAzu(dx * 0.005f);
		camera.incrementAlt(dy * 0.005f);
	}

	mouse_old_x = x;
	mouse_old_y = y;
}

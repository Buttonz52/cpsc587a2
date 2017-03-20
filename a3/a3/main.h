#include "Particle.h"
#include "Spring.h"
#include "Shader.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>
#include <vector>

#define PI 3.14159265359f
#define gravity 9.81f

#define defaultZoom 8.f

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

using namespace glm;
using namespace std;

//Simulation
float delta_t = 0.01;
float curr_t = 0.00;
vector<Particle*> particles;
vector<Spring*> springs;


//functions
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void errorCallback(int error, const char* description);
void printOpenGLVersion();

void setupScene();
void simulate();
void connectSprings();
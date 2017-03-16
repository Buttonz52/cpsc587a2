#include "Camera.h"
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

//variables
const glm::mat4 ident(1.0f);
const GLfloat clearColor[] = { 0.f, 0.f, 0.f };
mat4 winRatio = mat4(1.f);

double  mouse_old_x,
mouse_old_y;

float   rotate_x = 0.0,
rotate_y = 0.0,
zoom = 5.f,
aspectRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

glm::vec3 up(0.f, 1.f, 0.f),
cam(0.f, 0.2f, 2.f),
center(0.f, 0.f, 0.f);

//
Camera camera;

//Simulation
float delta_t = 0.01;
float curr_t = 0.00;
vector<Particle*> particles;
vector<Spring*> springs;


//functions
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll(GLFWwindow* window, double x, double y);
void errorCallback(int error, const char* description);
void window_size_callback(GLFWwindow* window, int width, int height);
void motion(GLFWwindow* window, double x, double y);
void printOpenGLVersion();

void setupScene();
void simulate(vector<Spring*> s, vector<Particle*> p);
void connectSprings(vector<Spring*> s, vector<Particle*> p);
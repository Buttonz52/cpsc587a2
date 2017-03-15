#include "Camera.h"


Camera::Camera()
{
	setInitValues();
	center = vec3(0);
	focalPoint = glm::vec3(0, 0, 0);
}

Camera::Camera(vec3 &c) {
	setInitValues();
	focalPoint = glm::vec3(0, 0, 0);
	center = c;
}
Camera::Camera(vec3 &c, vec3 &f) {
	setInitValues();
	center = c;
	focalPoint = f;
}
Camera::~Camera() {}

void Camera::setInitValues() {
	azu = M_PI_4;
	alt = M_PI_4 * 1.5;
	radius = 35;

	fov = M_PI / 3;
	_near = 0.01;
	_far = 100;

	asp = (float)1920 / (float)1080;
}

void Camera::setCenter(vec3 &c) {
	center = c;
}
void Camera::setAlt(float newAlt)
{
	alt = newAlt;
	alt = min(max(alt, 0.000001f), M_PI - 0.000001f);
}

void Camera::setAzu(float newAzu)
{
	azu = newAzu;
}

void Camera::setAsp(float newAsp)
{
	asp = newAsp;
}

void Camera::incrementAlt(float newAlt)
{
	alt += newAlt;
	alt = min(max(alt, 0.001f), M_PI - 0.001f);
}

void Camera::incrementAzu(float newAzu)
{
	azu += newAzu;
}

void Camera::setRadius(float newRad)
{
	radius = newRad;
}

void Camera::incrementRadius(double newRad)
{
	radius -= newRad;
	radius = min(max(radius, 6.0f), 200.0f);
}

mat4 Camera::calculateProjectionMatrix()
{
	float dy = _near * tan(fov * 0.5);
	float dx = dy * asp;

	mat4 proj = perspective(fov, asp, _near, _far);

	return proj;
}

void Camera::translate3D(vec3 delta)
{
	vec4 temp = vec4(focalPoint, 0);
	temp = temp + vec4(delta, 0) * calculateViewMatrix();

	focalPoint = vec3(temp);
}

mat4 Camera::calculateViewMatrix()
{
	// Calculate x,y,z from spherical coordinates
	float x = radius * sin(alt) * cos(azu - M_PI / 2);
	float y = radius * cos(alt);
	float z = radius * sin(alt) * sin(azu - M_PI / 2);

	vec3 eye(x, y, z);
	vec3 up(0.0f, 1.0f, 0.0f);

	mat4 view = lookAt(eye, center, up);

	view = translate(view, focalPoint);

	return view;
}

vec3* Camera::getCenter()
{
	return &center;
}
#include "Time.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Time::Init() {
	time = glfwGetTime();
	last_time = time;
	delta_time = 0;
}
void Time::Update() {
	time = glfwGetTime();
	delta_time = time - last_time;
	last_time = time;
}

double Time::time = 0;
double Time::last_time = 0;
double Time::delta_time = 0;
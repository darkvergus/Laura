#include <GLFW/glfw3.h>
#include "deltaLib/deltaLib.h"

DeltaTime::DeltaTime()
	: prev_time(0.0f), curr_time(0.0f) {}

double DeltaTime::getDelta()
{
	double delta_time = curr_time - prev_time;
	return delta_time;
}

void DeltaTime::update()
{
	prev_time = curr_time;
	curr_time = glfwGetTime();
}
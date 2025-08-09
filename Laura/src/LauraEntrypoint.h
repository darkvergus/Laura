#pragma once

#include "Core/application.h"

extern Laura::Application* Laura::CreateApplication(); // new heap allocated polymorphic application (
int main() {
	Laura::Application* app = Laura::CreateApplication();
	app->run();
	delete app;
}
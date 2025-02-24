#pragma once
#include <core/application.h>

extern Laura::Application* Laura::CreateApplication(); // new heap allocated polymorphic application (

int main()
{
	Laura::Application* app = Laura::CreateApplication();
	app->run();
	delete app;
}
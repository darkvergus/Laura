#include <core/application.h>

extern Laura::Application* Laura::CreateApplication();

int main()
{
	Laura::Application* app = Laura::CreateApplication();
	app->run();
	delete app;
}
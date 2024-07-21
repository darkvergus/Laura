#include <core/application.h>


extern Laura::Application * Laura::createApplication();

int main()
{
	Laura::Application* app = Laura::createApplication();
	app->run();
	delete app;
	return 0;
}
#include <core/application.h>


extern Application* createApplication();

int main()
{
	Application* app = createApplication();
	app->run();
	delete app;
	return 0;
}
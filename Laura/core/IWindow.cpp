#include "core/IWindow.h"

#define PLATFORM_WINDOWS


#ifdef PLATFORM_WINDOWS
#include "platform/windows/GLFWWindow.h"
#endif



IWindow* IWindow::createWindow(WindowProps windowProps)
{
	#ifdef PLATFORM_WINDOWS
		return new GLFWWindowIMPL(windowProps);
	#else
		std::cout << "Unknown platform!" << std::endl;
		return nullptr;
	#endif
}

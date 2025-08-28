#include "core/IWindow.h"
#include "platform/windows/GLFWWindow.h"

namespace Laura
{

	std::shared_ptr<IWindow> IWindow::createWindow(WindowProps windowProps = WindowProps{}) {
		return std::make_shared<GLFWWindowIMPL>(windowProps);
	}
}
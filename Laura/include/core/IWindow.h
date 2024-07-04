#ifndef IWINDOW_H
#define IWINDOW_H

#include <string>
#include <functional>

#include "core/Events.h"

#include "core/KeyCodes.h"
#include "core/MouseCodes.h"

struct WindowProps
{
	std::string title;
	int width;
	int height;
	bool VSync;

	WindowProps(const std::string& title = std::string("Ray Tracing"),
		int width = 1600, 
		int height = 900,
		bool VSync = false)
		: width(width), height(height), title(title), VSync(VSync)
	{
	}
};

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual void onUpdate() = 0;

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	
	virtual void* getNativeWindow() const = 0;
	virtual void setVSync(bool enabled) = 0;
	virtual bool isVSync() const = 0;

	/// input polling
	virtual bool isKeyPressed(KeyCode key) = 0;
	virtual bool isMouseButtonPressed(MouseCode) = 0;
	virtual std::pair<float, float> getMousePosition() = 0;
	virtual bool shouldClose() = 0;

	// expects a function that takes an Event* as a parameter and returns void
	virtual void setEventCallback(const std::function<void(Event*)>& callback) = 0;
	
	/// createWindow is a factory method that creates a window with the given properties.
	/// reutrns a heap pointer
	static IWindow* createWindow(const WindowProps windowProps = WindowProps());
};

#endif // IWINDOW_H
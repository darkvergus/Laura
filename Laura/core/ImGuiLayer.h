#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "core/ILayer.h"
#include "core/IWindow.h"

class ImGuiLayer : public ILayer
{
public:
	ImGuiLayer(IWindow* window);
	virtual ~ImGuiLayer();

	void Begin();
	void End();

	void onAttach() override;
	void onDetach() override;
	void onImGuiRender() override;
private:
	IWindow* m_Window;
};


#endif // IMGUI_LAYER_H
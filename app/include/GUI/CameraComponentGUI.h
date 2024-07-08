#pragma once
#include "camera/Camera.h"
#include <imgui.h>
#include <unordered_map>

void scheduleKeybindChange(Camera& camera, int& keybind);
void component_cameraGUI(Camera& camera, bool& was_IMGUI_Input, bool disabled, bool& shouldAccumulate, bool& doPostProc, int& raysPerPixel, int& bouncesPerRay);
void genCameraChangeKeybindSplashScreen();
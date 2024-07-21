#pragma once
#include "camera/CameraOld.h"
#include <imgui.h>
#include <unordered_map>

void scheduleKeybindChange(Laura::Camera& camera, int& keybind);
void component_cameraGUI(Laura::Camera& camera, bool& was_IMGUI_Input, bool disabled, bool& shouldAccumulate, bool& doPostProc, int& raysPerPixel, int& bouncesPerRay);
void genCameraChangeKeybindSplashScreen();
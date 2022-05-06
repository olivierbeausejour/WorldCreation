#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <array>
#include <map>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxImGui.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

namespace EditorUI
{
	using namespace ofxImGui;
	using namespace ImGui;
}

#if defined(_WIN32) || defined(_WIN64)
	#include <Windows.h>
#endif // WC_PLATFORM_WINDOWS

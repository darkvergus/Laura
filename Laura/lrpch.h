#ifndef LRPCH_H
#define LRPCH_H

	#include <memory>
	#include <utility>
	#include <functional>
	#include <cstdint>

	#include <iostream>
	#include <sstream>
	#include <ostream>
	#include <fstream>

	#include <string>
	#include <vector>
	#include <queue>
	#include <unordered_map>
	#include <unordered_set>

	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#define GLM_ENABLE_EXPERIMENTAL
	#include <glm/gtx/string_cast.hpp>

	#include <algorithm>

	#include "core/Log.h"
	
	#ifdef LR_PLATFORM_WINDOWS
		#define NOMINMAX // Prevent windows.h from defining min and max macros that conflict with std::min and std::max
		#include <Windows.h>
	#endif

#endif // LRPCH_H

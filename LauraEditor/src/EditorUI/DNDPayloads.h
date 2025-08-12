#pragma once

#include <Laura.h>

namespace Laura {

	namespace DNDPayloadTypes {
		inline constexpr const char* MESH = "DND_PAYLOAD_MESH";
		inline constexpr const char* TEXTURE = "DND_PAYLOAD_TEXTURE";
		inline constexpr const char* SCENE = "DND_PAYLOAD_SCENE";
	}

	struct DNDPayload {
		LR_GUID guid;
		char title[256];
	};
}

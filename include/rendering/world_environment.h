#pragma once

namespace NocEngine {
	struct WorldEnvironment {
		glm::vec3 ambient_light{0.25f};
		float ambient_light_strength{1.f};
	};
}
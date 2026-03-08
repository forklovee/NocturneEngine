#pragma once

namespace NocEngine {

	struct CLightComponent : CComponent {
		glm::vec3 light_color{ 1.f };
		float light_strength{ 1.f };
	};

}
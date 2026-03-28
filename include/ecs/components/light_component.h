#pragma once

namespace NocEngine {

	struct CLightComponent : CComponent {
		glm::vec3 ambientColor{ 1.f };
		glm::vec3 diffuseColor{ 1.f };
		glm::vec3 specularColor{ 1.f };
	};

}
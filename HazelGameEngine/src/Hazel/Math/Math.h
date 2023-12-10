#pragma once

#include <glm/glm.hpp>

namespace Hazel::Math {

	// Transform mat4 에서 s, r, t 정보를 뽑아내주는 함수
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

}

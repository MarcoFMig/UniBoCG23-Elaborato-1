#pragma once

#include <vector>
#include <utility>
#include <glm/vec3.hpp>

namespace SimpleMath {
  float differenceQuotient(std::pair<float, float> points, std::pair<float, float> interval);
  float lerp(float a, float b, float f);
};

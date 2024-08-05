#include "simplemath.hpp"

#include <cmath>
#include <vector>
#include <utility>
#include <glm/vec3.hpp>

float SimpleMath::differenceQuotient(std::pair<float, float> points, std::pair<float, float> interval) {
  return (points.second - points.first) / (interval.second - interval.first);
}

float SimpleMath::lerp(float a, float b, float f) {
  return a + f * (b - a);
}

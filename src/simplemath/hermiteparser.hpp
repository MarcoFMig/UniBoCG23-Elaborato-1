#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

class HermiteParser {
  public:
    std::vector<glm::vec3> readHermite(std::string filename);
    std::vector<glm::vec3> readHermite(const char* filename);

    void dumpHermite(std::vector<glm::vec3> controlPoints);
};

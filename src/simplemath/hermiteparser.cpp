#include "hermiteparser.hpp"

#include <vector>
#include <fstream>
#include <exception>

std::vector<glm::vec3> HermiteParser::readHermite(const char* filename) {
  std::ifstream hermiteFile;
  hermiteFile.open(filename);
  float x = 0;
  float y = 0;
  float z = 0;
  if (!hermiteFile.is_open()) {
    throw std::system_error();
  }
  std::vector<glm::vec3> resultList;
  while (hermiteFile >> x >> y >> z) {
    resultList.push_back(glm::vec3(x, y, z));
  }
  hermiteFile.close();
  return resultList;
}

std::vector<glm::vec3> HermiteParser::readHermite(std::string filename) {
  return this->readHermite(filename.c_str());
}

void HermiteParser::dumpHermite(std::vector<glm::vec3> controlPoints) {
  // TODO: Finish this implementation.
}

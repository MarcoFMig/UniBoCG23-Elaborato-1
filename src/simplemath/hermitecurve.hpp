#pragma once

#include <vector>
#include <string>
#include <utility>
#include <glm/vec3.hpp>

class HermiteCurve {
  public:
    struct TCBParams {
      float tension;
      float continuity;
      float bias;
      TCBParams(float t = 0.0f, float c = 0.0f, float b = 0.0f)
        : tension(t), continuity(c), bias(b) {}
    };
  private:
    float step;
    float curveStep;
    unsigned int curveDefinition;
    std::vector<std::pair<glm::vec3, TCBParams>> controlPoints;
    std::vector<glm::vec3> curvePoints;
    void initializeParams();
    float interpolate(unsigned int leftIntervalExt, std::vector<float>& curveIntervals, unsigned short versor);
  public:
    HermiteCurve(std::vector<std::pair<glm::vec3, TCBParams>> controlPoints);
    HermiteCurve(std::vector<glm::vec3> controlPoints);
    HermiteCurve(std::string fileToRead);
    std::vector<std::pair<glm::vec3, TCBParams>> getControlPoints();
    std::vector<glm::vec3> getCurvePoints(glm::vec3 center);
    std::vector<glm::vec3> getCurvePoints();
    void calculate();
    void serializeCurve(bool humanReadable);
};

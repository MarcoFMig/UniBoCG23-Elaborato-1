#include "hermitecurve.hpp"

#include <utility>
#include <glm/vec3.hpp>

#include "simplemath.hpp"
#include "hermiteparser.hpp"

#define __DEFINITION_PER_INTERVAL 10
#define __DIMENSIONS 2

HermiteCurve::HermiteCurve(std::vector<std::pair<glm::vec3, TCBParams>> controlPoints) {
  this->controlPoints = controlPoints;
  this->initializeParams();
}
HermiteCurve::HermiteCurve(std::vector<glm::vec3> controlPoints) {
  this->controlPoints.reserve(controlPoints.size());
  for (glm::vec3 controlPoint : controlPoints) {
    std::pair<glm::vec3, TCBParams> valuePair;
    valuePair.first = controlPoint;
    valuePair.second = TCBParams();
    this->controlPoints.push_back(valuePair);
  }
  this->initializeParams();
}
HermiteCurve::HermiteCurve(std::string filename) {
  HermiteParser parser;
  std::vector<glm::vec3> controlPoints = parser.readHermite(filename);
  for (glm::vec3 controlPoint : controlPoints) {
    std::pair<glm::vec3, TCBParams> valuePair;
    valuePair.first = controlPoint;
    valuePair.second = TCBParams();
    this->controlPoints.push_back(valuePair);
  }
  this->initializeParams();
}

std::vector<std::pair<glm::vec3, HermiteCurve::TCBParams>> HermiteCurve::getControlPoints() {
  return this->controlPoints;
}
std::vector<glm::vec3> HermiteCurve::getCurvePoints() {
  return this->curvePoints;
}
std::vector<glm::vec3> HermiteCurve::getCurvePoints(glm::vec3 center) {
  std::vector<glm::vec3> pointsWithCenter;
  pointsWithCenter.reserve(this->curvePoints.size() + 1);
  pointsWithCenter.push_back(center);
  pointsWithCenter.insert(pointsWithCenter.end(), this->curvePoints.begin(), this->curvePoints.end());
  return pointsWithCenter;
}

void HermiteCurve::initializeParams() {
  this->curveDefinition = this->controlPoints.size() * (__DEFINITION_PER_INTERVAL - 1);
  this->curvePoints.reserve(this->curveDefinition);
  this->curveStep = 1.0f / this->curveDefinition;
  this->step = 1.0f / this->controlPoints.size();
}

void HermiteCurve::calculate() {
  // Interval definition
  std::vector<float> curveIntervals;
  curveIntervals.reserve(this->controlPoints.size());
  float step = 1.0f / (this->controlPoints.size() - 1);
  for (unsigned int index = 0; index < this->controlPoints.size(); index ++) {
    curveIntervals.push_back(index * step);
  }

  // Sub-intervals definition
  unsigned int currentInterval = 0;
  for (float intervalPosition = 0.0f; intervalPosition <= 1; intervalPosition += this->curveStep) {
    if (intervalPosition > curveIntervals[currentInterval + 1]) {
      currentInterval ++;
    }
    float amplitude = curveIntervals[currentInterval + 1] - curveIntervals[currentInterval];
    float tangentMapp = (intervalPosition - curveIntervals[currentInterval]) / amplitude;
    glm::vec3 point(0.0f, 0.0f, 0.0f);

    float phi0 = 2.0f * tangentMapp*tangentMapp*tangentMapp
      - 3.0f *tangentMapp*tangentMapp + 1;
    float phi1 = tangentMapp*tangentMapp*tangentMapp
      - 2.0f * tangentMapp*tangentMapp + tangentMapp;
    float psi0 = -2.0f * tangentMapp*tangentMapp*tangentMapp
      + 3.0f * tangentMapp*tangentMapp;
    float psi1 = tangentMapp*tangentMapp*tangentMapp
      - tangentMapp*tangentMapp;
    
    for (unsigned short versor = 0; versor < __DIMENSIONS; versor ++) {
      float coordinate = this->controlPoints[currentInterval].first[versor];
      float nextCoordinate = this->controlPoints[currentInterval + 1].first[versor];
      point[versor] = coordinate * phi0 + interpolate(currentInterval, curveIntervals, versor)
        * phi1 * amplitude + nextCoordinate * psi0
        + interpolate(currentInterval + 1, curveIntervals, versor) * psi1 * amplitude;
    }
    this->curvePoints.push_back(point);
  }
}

float HermiteCurve::interpolate(unsigned int leftIntervalExt, std::vector<float>& curveIntervals, unsigned short versor) {
  float tension = this->controlPoints[leftIntervalExt].second.tension;
  float bias = this->controlPoints[leftIntervalExt].second.bias;
  float continuity = this->controlPoints[leftIntervalExt].second.continuity;
  if (leftIntervalExt == 0) {
    return 0.5f * (1.0f - tension) * (1.0f - bias) * (1.0f - continuity)
     * SimpleMath::differenceQuotient(
      std::make_pair(
        this->controlPoints[leftIntervalExt + 1].first[versor],
        this->controlPoints[leftIntervalExt].first[versor]),
      std::make_pair(
        curveIntervals[leftIntervalExt + 1],
        curveIntervals[leftIntervalExt]));
  } else if (leftIntervalExt == this->controlPoints.size() - 1) {
    return 0.5f * (1.0f - tension) * (1.0f - bias) * (1.0f - continuity)
      * SimpleMath::differenceQuotient(
        std::make_pair(
          this->controlPoints[leftIntervalExt].first[versor],
          this->controlPoints[leftIntervalExt - 1].first[versor]),
        std::make_pair(
          curveIntervals[leftIntervalExt],
          curveIntervals[leftIntervalExt - 1]));
  }

  if (leftIntervalExt % 2 == 0) {
    return 0.5f * (1.0f - tension) * (1.0f + bias) * (1.0f + continuity)
      * SimpleMath::differenceQuotient(
          std::make_pair(
            this->controlPoints[leftIntervalExt].first[versor],
            this->controlPoints[leftIntervalExt - 1].first[versor]),
          std::make_pair(
            curveIntervals[leftIntervalExt],
            curveIntervals[leftIntervalExt - 1]))
      + 0.5f * (1.0f - tension) * (1.0f - bias) * (1.0f - continuity)
      * SimpleMath::differenceQuotient(
          std::make_pair(
            this->controlPoints[leftIntervalExt + 1].first[versor],
            this->controlPoints[leftIntervalExt].first[versor]),
          std::make_pair(
            curveIntervals[leftIntervalExt + 1],
            curveIntervals[leftIntervalExt]));
  } else {
    return 0.5f * (1.0f - tension) * (1.0f + bias) * (1.0f - continuity)
      * SimpleMath::differenceQuotient(
          std::make_pair(
            this->controlPoints[leftIntervalExt].first[versor],
            this->controlPoints[leftIntervalExt - 1].first[versor]),
          std::make_pair(
            curveIntervals[leftIntervalExt],
            curveIntervals[leftIntervalExt - 1]))
      + 0.5f * (1.0f - tension) * (1.0f - bias) * (1.0f + continuity)
      * SimpleMath::differenceQuotient(
          std::make_pair(
            this->controlPoints[leftIntervalExt + 1].first[versor],
            this->controlPoints[leftIntervalExt].first[versor]),
          std::make_pair(
            curveIntervals[leftIntervalExt + 1],
            curveIntervals[leftIntervalExt]));
  }
}

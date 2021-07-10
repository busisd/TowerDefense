#include "./TDSimulation.h"

#include <SDL2/SDL_scancode.h>

#include <cmath>

// Point

TowerDefense::Point::Point(double x, double y) {
  this->x_ = x;
  this->y_ = y;
}

double TowerDefense::Point::getX() {
  return x_;
}

double TowerDefense::Point::getY() {
  return y_;
}

double TowerDefense::Point::DistanceTo(Point p2) {
  return std::sqrt(std::pow(p2.getX() - x_, 2.0) + std::pow(p2.getY() - y_, 2.0));
}

// Path

TowerDefense::Path::Path(std::vector<std::pair<double, double>> &inputPoints) {
  for (auto point : inputPoints) {
    points_.emplace_back(Point(point.first, point.second));
  }

  for (int i = 0; i < inputPoints.size() - 1; i++) {
    segmentLengths_.emplace_back(points_.at(i).DistanceTo(points_.at(i + 1)));
  }

  double pathLength = 0;
  for (double segmentLength : segmentLengths_) {
    pathLength += segmentLength;
  }
  pathLength_ = pathLength;
}

TowerDefense::Point TowerDefense::Path::PointAt(double distance) {
  int curIndex = 0;
  double curSegmentLength = segmentLengths_.at(curIndex);

  while (curSegmentLength < distance) {
    distance -= curSegmentLength;

    curIndex = (curIndex + 1) % segmentLengths_.size();
    curSegmentLength = segmentLengths_.at(curIndex);
  }

  double percentThroughSegment = distance / curSegmentLength;
  Point p1 = points_.at(curIndex);
  Point p2 = points_.at(curIndex + 1);
  double newX = p1.getX() + (p2.getX() - p1.getX()) * percentThroughSegment;
  double newY = p1.getY() + (p2.getY() - p1.getY()) * percentThroughSegment;
  return Point(newX, newY);
}

double TowerDefense::Path::getLength() {
  return pathLength_;
}

// Simulation

TowerDefense::TDSimulation::TDSimulation(std::vector<std::pair<double, double>> path) : path_(path) {
}

void TowerDefense::TDSimulation::initialize() {
  playerX = 50;
  playerY = 50;
  enemyDist_ = 0;
}

void TowerDefense::TDSimulation::update(TDInputs &inputs) {
  if (inputs.keystates[SDL_SCANCODE_LEFT])
    playerX -= 5;
  if (inputs.keystates[SDL_SCANCODE_RIGHT])
    playerX += 5;
  if (inputs.keystates[SDL_SCANCODE_UP])
    playerY -= 5;
  if (inputs.keystates[SDL_SCANCODE_DOWN])
    playerY += 5;

  enemyDist_ += 3;
  if (enemyDist_ > path_.getLength()) enemyDist_ -= path_.getLength();
}

void TowerDefense::TDSimulation::cleanup() {
}

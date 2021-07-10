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

// Enemy

TowerDefense::Enemy::Enemy(EnemyData data) {
  type_ = data.type;
  max_health_ = data.maxHealth;
  current_health_ = max_health_;
  speed_ = data.speed;
  dist_ = 0;
}

double TowerDefense::Enemy::Move() {
  dist_ += speed_;
  return dist_;
}

double TowerDefense::Enemy::TakeDamage(double damageAmount) {
  current_health_ -= damageAmount;
  return current_health_;
}

TowerDefense::EnemyType TowerDefense::Enemy::getType() {
  return type_;
}
double TowerDefense::Enemy::getDist() {
  return dist_;
}
double TowerDefense::Enemy::getSpeed() {
  return speed_;
}
double TowerDefense::Enemy::getMaxHealth() {
  return max_health_;
}
double TowerDefense::Enemy::getCurrentHealth() {
  return current_health_;
}

void TowerDefense::Enemy::setType(EnemyType type) {
  type_ = type;
}

void TowerDefense::Enemy::setDist(double dist) {
  dist_ = dist;
}

void TowerDefense::Enemy::setSpeed(double speed) {
  speed_ = speed;
}

void TowerDefense::Enemy::setMaxHealth(double maxHealth) {
  max_health_ = maxHealth;
}

void TowerDefense::Enemy::setCurrentHealth(double currentHealth) {
  current_health_ = currentHealth;
}

// Simulation

TowerDefense::TDSimulation::TDSimulation(std::vector<std::pair<double, double>> path) : path_(path) {
  enemies_.emplace_back(kEnemyStats.find(EnemyType::Goblin)->second);
  enemies_.emplace_back(kEnemyStats.find(EnemyType::Smiley)->second);
  enemies_.emplace_back(kEnemyStats.find(EnemyType::Goblin)->second);
  enemies_.at(2).setDist(100);
}

void TowerDefense::TDSimulation::initialize() {
  playerX = 50;
  playerY = 50;
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

  for (Enemy &enemy : enemies_) {
    double newDistance = enemy.Move();
    if (newDistance > path_.getLength()) enemy.setDist(0);
  }
}

void TowerDefense::TDSimulation::cleanup() {
}

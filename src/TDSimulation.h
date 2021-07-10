#ifndef TD_SIMULATION_H
#define TD_SIMULATION_H

#include <unordered_map>
#include <vector>

#include "./TDInputs.h"

namespace TowerDefense {

class Point {
 public:
  Point(double x, double y);
  double getX();
  double getY();
  double DistanceTo(Point p2);

 private:
  double x_;
  double y_;
};

class Path {
  friend class TDRenderer;

 public:
  Path(std::vector<std::pair<double, double>> &inputPoints);
  Point PointAt(double distance);
  double getLength();

 private:
  std::vector<TowerDefense::Point> points_;
  std::vector<double> segmentLengths_;
  double pathLength_;
};

enum class EnemyType {
  Smiley,
  Goblin
};

struct EnemyData {
  EnemyType type;
  double speed;
  double maxHealth;
};

const std::unordered_map<EnemyType, EnemyData> kEnemyStats = {
    {EnemyType::Smiley, (EnemyData){EnemyType::Smiley, 2.5, 50}},
    {EnemyType::Goblin, (EnemyData){EnemyType::Goblin, 1.3, 100}}};

class Enemy {
 public:
  Enemy(EnemyData data);

  double Move();
  double TakeDamage(double damageAmount);

  EnemyType getType();
  double getDist();
  double getSpeed();
  double getMaxHealth();
  double getCurrentHealth();

  void setType(EnemyType type);
  void setDist(double dist);
  void setSpeed(double speed);
  void setMaxHealth(double maxHealth);
  void setCurrentHealth(double currentHealth);

 private:
  EnemyType type_;
  double dist_;
  double speed_;
  double max_health_;
  double current_health_;
};

class TDSimulation {
 public:
  TDSimulation(std::vector<std::pair<double, double>> path);

  void initialize();

  void update(TDInputs &inputs);

  void cleanup();

  int playerX;
  int playerY;
  Path path_;
  std::vector<Enemy> enemies_;
};

}  // namespace TowerDefense

#endif

#ifndef TD_SIMULATION_H
#define TD_SIMULATION_H

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

class TDSimulation {
 public:
  TDSimulation(std::vector<std::pair<double, double>> path);

  void initialize();

  void update(TDInputs &inputs);

  void cleanup();

  int playerX;
  int playerY;
  Path path_;
  int enemyDist_;
};

}  // namespace TowerDefense

#endif

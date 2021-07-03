#include <SDL2/SDL.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace TowerDefense {

enum SPRITE_ENUM {
  SmileSprite
};

class TDInputs {
 public:
  void initialize() {
    this->keystates = SDL_GetKeyboardState(NULL);
  }

  bool processEvents() {
    bool shouldContinue = true;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          shouldContinue = false;
      }
    }

    if (keystates[SDL_SCANCODE_ESCAPE]) {
      shouldContinue = false;
    }

    return shouldContinue;
  }

  void cleanup() {
  }

  const Uint8 *keystates;
};

class Point {
 public:
  Point(double x, double y) {
    this->x_ = x;
    this->y_ = y;
  }

  double getX() {
    return x_;
  }

  double getY() {
    return y_;
  }

  double DistanceTo(Point p2) {
    return std::sqrt(std::pow(p2.getX() - x_, 2.0) + std::pow(p2.getY() - y_, 2.0));
  }

 private:
  double x_;
  double y_;
};

class Path {
  friend class TDRenderer;

 public:
  Path(std::vector<std::pair<double, double>> &inputPoints) {
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

  Point PointAt(double distance) {
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

  double getLength() {
    return pathLength_;
  }

 private:
  std::vector<TowerDefense::Point> points_;
  std::vector<double> segmentLengths_;
  double pathLength_;
};

class TDSimulation {
 public:
  TDSimulation(std::vector<std::pair<double, double>> path) : path_(path) {
  }

  void initialize() {
    playerX = 50;
    playerY = 50;
    enemyDist_ = 0;
  }

  void update(TDInputs &inputs) {
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

  void cleanup() {
  }

  int playerX;
  int playerY;
  Path path_;
  int enemyDist_;
};

class TDRenderer {
 public:
  void initialize() {
    SDL_Init(SDL_INIT_EVERYTHING);
    this->window = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    this->renderer = SDL_CreateRenderer(window, -1, 0);

    // TODO: Initialize all the sprites into an undordered_map

    // temp
    SDL_Surface *sprite1_surface = SDL_LoadBMP("./img/sprite1.bmp");
    this->sprite1 = SDL_CreateTextureFromSurface(renderer, sprite1_surface);
    if (sprite1_surface == nullptr) {
      std::cerr << "Error: could not load sprite1" << std::endl;
    }
    SDL_FreeSurface(sprite1_surface);
    if (sprite1 == nullptr) {
      std::cerr << "Error: could not turn sprite1 surface into texture" << std::endl;
    }
    rect1.h = 80;
    rect1.w = 80;
    // end temp

    SDL_SetRenderDrawColor(renderer, 176, 245, 236, 255);
  }

  void render(TDSimulation &game) {
    SDL_SetRenderDrawColor(renderer, 176, 245, 236, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int i = 0; i < game.path_.points_.size() - 1; i++) {
      Point p1 = game.path_.points_[i];
      Point p2 = game.path_.points_[i + 1];
      SDL_RenderDrawLineF(renderer, p1.getX(), p1.getY(), p2.getX(), p2.getY());
    }

    // TODO: Render all the sprites
    // for (sprite : sprites) {
    rect1.x = game.playerX;
    rect1.y = game.playerY;
    SDL_RenderCopy(renderer, sprite1, NULL, &rect1);

    Point enemyPosition = game.path_.PointAt(game.enemyDist_);
    rect1.x = enemyPosition.getX() - rect1.w / 2;
    rect1.y = enemyPosition.getY() - rect1.h / 2;
    SDL_RenderCopy(renderer, sprite1, NULL, &rect1);
    //}

    SDL_RenderPresent(renderer);
  }

  void cleanup() {
    // TODO: Free and destroy all the sprites

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

 private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  std::unordered_map<SPRITE_ENUM, SDL_Texture *> sprites;

  // temp:
  SDL_Texture *sprite1;
  SDL_Rect rect1;
};

}  // namespace TowerDefense

const int FRAME_TIME_MS = 16;

int main(int argv, char **args) {
  bool running = true;

  std::vector<std::pair<double, double>> gamePath = {
      {100, 100},
      {400, 100},
      {400, 200},
      {200, 300},
      {300, 300},
      {500, 300},
      {100, 500},
      {700, 500},
      {700, 100},
  };

  TowerDefense::TDSimulation simulation = TowerDefense::TDSimulation(gamePath);
  TowerDefense::TDRenderer renderer;
  TowerDefense::TDInputs inputs;

  simulation.initialize();  // TODO: Check for return = 1 for errors
  renderer.initialize();
  inputs.initialize();

  auto gameClock = std::chrono::high_resolution_clock();
  auto realTime = gameClock.now();
  auto simulationTime = gameClock.now();

  auto frameTime = std::chrono::milliseconds(16);

  while (running) {
    realTime = gameClock.now();

    while (simulationTime < realTime) {
      running = inputs.processEvents();
      simulation.update(inputs);

      simulationTime += frameTime;
    }

    renderer.render(simulation);  //TODO: Interpolation maybe
  }

  inputs.cleanup();
  simulation.cleanup();
  renderer.cleanup();

  return 0;
}

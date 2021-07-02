#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <vector>

class GameInputs {
  void initialize() {
  }

  void getInputs(std::vector<int> &inputs) {
  }

  void cleanup() {
  }
};

class GameSimulation {
  void initialize() {
  }

  void update() {
  }

  void cleanup() {
  }
};

class GameRenderer {
  void initialize() {
  }

  void render(GameSimulation &game) {
  }

  void cleanup() {
  }
};

const int FRAME_TIME_MS = 16;

int main(int argv, char **args) {
  bool running = true;

  // GameSimulation mySimulation;
  // GameRenderer myRenderer;

  // mySimulation.initialize();
  // myRenderer.initialize();

  auto gameClock = std::chrono::high_resolution_clock();
  auto realTime = gameClock.now();
  auto simulationTime = gameClock.now();

  auto frameTime = std::chrono::milliseconds(16);

  while (running) {
    realTime = gameClock.now();

    while (simulationTime < realTime) {
      // mySimulation.update();
      simulationTime += frameTime;
    }

    // mySimulation.render();
  }

  // mySimulation.cleanup();
  // myRenderer.cleanup();

  return 0;
}

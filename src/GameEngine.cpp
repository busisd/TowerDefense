#include <chrono>
#include <iostream>

const int FRAME_TIME_MS = 16;

int main(int argv, char **args) {
  std::cout << "TEST" << std::endl;

  bool running = true;

  // GameSimulation mySimulation;
  // GameRenderer myRenderer;

  // mySimulation.initialize();

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

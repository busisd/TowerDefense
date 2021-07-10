#include <chrono>

#include "./TDInputs.h"
#include "./TDRenderer.h"
#include "./TDSimulation.h"

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

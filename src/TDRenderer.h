#ifndef TD_RENDERER_H
#define TD_RENDERER_H

#include <SDL2/SDL.h>

#include <unordered_map>

#include "./TDSimulation.h"

namespace TowerDefense {

// TODO?: Possibly include img size here
const std::unordered_map<EnemyType, std::string> kEnemyFilenames = {
    {EnemyType::Smiley, "./img/smiley.bmp"},
    {EnemyType::Goblin, "./img/goblin.bmp"}};

class TDRenderer {
 public:
  void initialize();

  void render(TDSimulation &game);

  void cleanup();

 private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  std::unordered_map<EnemyType, SDL_Texture *> sprites_;
  SDL_Rect rect_;
};

}  // namespace TowerDefense

#endif

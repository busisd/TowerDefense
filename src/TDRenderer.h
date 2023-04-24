#ifndef TD_RENDERER_H
#define TD_RENDERER_H

#include <SDL2/SDL.h>
#include <string>

#include <unordered_map>

#include "./TDSimulation.h"

namespace TowerDefense {

// TODO?: Possibly include img size here
const std::unordered_map<EnemyType, std::vector<std::string>> kEnemyFilenames = {
    {EnemyType::Smiley, {"./img/smiley.bmp"}},
    {EnemyType::Monkey, {"./img/monkey0.bmp", "./img/monkey1.bmp", "./img/monkey2.bmp", "./img/monkey1.bmp"}}, //TODO: This is wasteful, implement patterns/timings for this
    {EnemyType::Goblin, {"./img/goblin0.bmp", "./img/goblin1.bmp"}}};

class AnimatedSprite {
 public:
  AnimatedSprite(EnemyType type, std::vector<std::string> frameFilenames, SDL_Renderer *renderer);
  ~AnimatedSprite();

  SDL_Texture *GetTexture(int frameNum);

 private:
  EnemyType type_;
  std::vector<SDL_Texture *> frames_;
  SDL_Texture * test1;
};

class TDRenderer {
 public:
  void initialize();
  void render(TDSimulation &game);
  void cleanup();

 private:
  SDL_Window *window_;
  SDL_Renderer *renderer_;
  std::unordered_map<EnemyType, AnimatedSprite *> sprites_;
  SDL_Rect rect_;
};

}  // namespace TowerDefense

#endif

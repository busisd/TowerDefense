#ifndef TD_RENDERER_H
#define TD_RENDERER_H

#include <SDL2/SDL.h>

#include <unordered_map>

#include "./TDSimulation.h"

namespace TowerDefense {

enum SPRITE_ENUM {
  SmileSprite
};

class TDRenderer {
 public:
  void initialize();

  void render(TDSimulation &game);

  void cleanup();

 private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  std::unordered_map<SPRITE_ENUM, SDL_Texture *> sprites;

  // temp:
  SDL_Texture *sprite1;
  SDL_Rect rect1;
};

}  // namespace TowerDefense

#endif

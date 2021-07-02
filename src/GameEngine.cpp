#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <unordered_map>

enum SPRITE_ENUM {
  SmileSprite
};

class TowerDefenseInputs {
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

class TowerDefenseSimulation {
 public:
  void initialize() {
    playerX = 50;
    playerY = 50;
  }

  void update(TowerDefenseInputs &inputs) {
    if (inputs.keystates[SDL_SCANCODE_LEFT])
      playerX -= 5;
    if (inputs.keystates[SDL_SCANCODE_RIGHT])
      playerX += 5;
    if (inputs.keystates[SDL_SCANCODE_UP])
      playerY -= 5;
    if (inputs.keystates[SDL_SCANCODE_DOWN])
      playerY += 5;
  }

  void cleanup() {
  }

  int playerX;
  int playerY;
};

class TowerDefenseRenderer {
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

  void render(TowerDefenseSimulation &game) {
    SDL_RenderClear(renderer);

    // TODO: Render all the sprites
    // for (sprite : sprites) {
      rect1.x = game.playerX;
      rect1.y = game.playerY;
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

const int FRAME_TIME_MS = 16;

int main(int argv, char **args) {
  bool running = true;

  TowerDefenseSimulation TDSimulation;
  TowerDefenseRenderer TDRenderer;
  TowerDefenseInputs TDInputs;

  TDSimulation.initialize(); // TODO: Check for return = 1 for errors
  TDRenderer.initialize();
  TDInputs.initialize();

  auto gameClock = std::chrono::high_resolution_clock();
  auto realTime = gameClock.now();
  auto simulationTime = gameClock.now();

  auto frameTime = std::chrono::milliseconds(16);

  while (running) {
    realTime = gameClock.now();

    while (simulationTime < realTime) {
      running = TDInputs.processEvents();
      TDSimulation.update(TDInputs);
      
      simulationTime += frameTime;
    }

    TDRenderer.render(TDSimulation); //TODO: Interpolation maybe
  }

  TDInputs.cleanup();
  TDSimulation.cleanup();
  TDRenderer.cleanup();

  return 0;
}

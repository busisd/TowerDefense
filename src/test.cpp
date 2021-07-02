#include <SDL2/SDL.h>

#include <iostream>

void destruct(SDL_Window *window, SDL_Renderer *renderer) {
  std::cout << "Destructing!" << std::endl;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argv, char **args) {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  bool isRunning = true;
  SDL_Event event;

  SDL_Surface *sprite1_surface = SDL_LoadBMP("./img/sprite1.bmp");
  if (sprite1_surface == nullptr) {
    std::cerr << "Error: could not load sprite1" << std::endl;
    destruct(window, renderer);
    return 1;
  }

  SDL_Texture *sprite1 = SDL_CreateTextureFromSurface(renderer, sprite1_surface);
  SDL_FreeSurface(sprite1_surface);
  if (sprite1 == nullptr) {
    std::cerr << "Error: could not turn sprite1 surface into texture" << std::endl;
    destruct(window, renderer);
    return 1;
  }

  SDL_Rect rect1;
  rect1.h = 80;
  rect1.w = 80;
  rect1.x = 50;
  rect1.y = 50;

  SDL_SetRenderDrawColor(renderer, 176, 245, 236, 255);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, sprite1, NULL, &rect1);
  SDL_RenderPresent(renderer);

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          isRunning = false;
          break;
      }
    }

    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_LEFT])
      rect1.x -= 5;
    if (keystates[SDL_SCANCODE_RIGHT])
      rect1.x += 5;
    if (keystates[SDL_SCANCODE_UP])
      rect1.y -= 5;
    if (keystates[SDL_SCANCODE_DOWN])
      rect1.y += 5;
    if (keystates[SDL_SCANCODE_ESCAPE])
      isRunning = false;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, sprite1, NULL, &rect1);
    SDL_RenderPresent(renderer);
    SDL_Delay((1.0 / 60) * 1000);
  }

  SDL_DestroyTexture(sprite1);
  destruct(window, renderer);

  return 0;
}

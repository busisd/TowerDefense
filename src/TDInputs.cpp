#include "./TDInputs.h"

void TowerDefense::TDInputs::initialize() {
  this->keystates = SDL_GetKeyboardState(NULL);
}

bool TowerDefense::TDInputs::processEvents() {
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

void TowerDefense::TDInputs::cleanup() {
}

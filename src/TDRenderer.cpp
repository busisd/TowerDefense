#include "./TDRenderer.h"

#include <iostream>

void TowerDefense::TDRenderer::initialize() {
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

void TowerDefense::TDRenderer::render(TDSimulation &game) {
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

void TowerDefense::TDRenderer::cleanup() {
  // TODO: Free and destroy all the sprites

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

#include "./TDRenderer.h"

#include <iostream>

SDL_Texture *LoadTexture(std::string filename, SDL_Renderer *renderer) {
  SDL_Surface *sprite_surface = SDL_LoadBMP(filename.c_str());
  if (sprite_surface == nullptr) {
    std::cerr << "Error: could not load sprite img file: " << filename << std::endl;
    //TODO: throw error
  }
  SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_FreeSurface(sprite_surface);
  if (sprite_texture == nullptr) {
    std::cerr << "Error: could not convert surface " << filename << " to texture" << std::endl;
    //TODO: throw error
  }

  return sprite_texture;
}

// AnimatedSprite

TowerDefense::AnimatedSprite::AnimatedSprite(EnemyType type, std::vector<std::string> frameFilenames, SDL_Renderer *renderer) {
  type_ = type;

  for (std::string filename : frameFilenames) {
    SDL_Texture *frameTexture = LoadTexture(filename, renderer);
    frames_.emplace_back(frameTexture);
  }
}

TowerDefense::AnimatedSprite::~AnimatedSprite() {
  for (SDL_Texture *texture : frames_) {
    SDL_DestroyTexture(texture);
  }
}

SDL_Texture *TowerDefense::AnimatedSprite::GetTexture(int frameNum) {
  std::cout << frames_.size() << std::endl;
  if (frames_.size() == 1) return frames_.at(0);



  return frames_.at(0);
}

// TDRenderer

void TowerDefense::TDRenderer::initialize() {
  SDL_Init(SDL_INIT_EVERYTHING);
  this->window_ = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
  this->renderer_ = SDL_CreateRenderer(window_, -1, 0);

  for (auto enemy : kEnemyFilenames) {
    // AnimatedSprite newSprite = AnimatedSprite(enemy.first, enemy.second, renderer_);
    SDL_Texture *newSprite = LoadTexture(enemy.second, renderer_);
    sprites_.emplace(enemy.first, newSprite);
  }

  rect_.h = 80;
  rect_.w = 80;
  SDL_SetRenderDrawColor(renderer_, 176, 245, 236, 255);
}

void TowerDefense::TDRenderer::render(TDSimulation &game) {
  SDL_SetRenderDrawColor(renderer_, 176, 245, 236, 255);
  SDL_RenderClear(renderer_);

  SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
  for (int i = 0; i < game.path_.points_.size() - 1; i++) {
    Point p1 = game.path_.points_[i];
    Point p2 = game.path_.points_[i + 1];
    SDL_RenderDrawLineF(renderer_, p1.getX(), p1.getY(), p2.getX(), p2.getY());
  }

  rect_.x = game.playerX;
  rect_.y = game.playerY;
  SDL_RenderCopy(renderer_, sprites_.find(EnemyType::Smiley)->second, NULL, &rect_);

  for (Enemy enemy : game.enemies_) {
    Point enemyPosition = game.path_.PointAt(enemy.getDist());
    rect_.x = enemyPosition.getX() - rect_.w / 2;
    rect_.y = enemyPosition.getY() - rect_.h / 2;
    SDL_RenderCopy(renderer_, sprites_.find(enemy.getType())->second, NULL, &rect_);
  }

  SDL_RenderPresent(renderer_);
}

void TowerDefense::TDRenderer::cleanup() {
  for (auto sprite : sprites_) {
    SDL_DestroyTexture(sprite.second);
  }

  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

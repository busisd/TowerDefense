#ifndef TD_INPUTS_H
#define TD_INPUTS_H

#include <SDL2/SDL.h>

namespace TowerDefense {

class TDInputs {
 public:
  void initialize();
  bool processEvents();
  void cleanup();

  const Uint8 *keystates;
};

}  // namespace TowerDefense

#endif

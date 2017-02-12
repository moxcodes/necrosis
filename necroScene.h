#ifndef NECROSCENE_H
#define NECROSCENE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "necroLayer.h"
#include "necroTile.h"

class necroScene{
  //because we need an overall registry somewhere
  float zoomFactor;
  float shiftValue[2];
 public:
  std::vector<SDL_Texture*>* tileText; //can be moved to private later
  std::vector<necroLayer*>* layers;  //can be migrated to private later
  necroScene();
  ~necroScene();
  void renderScene(SDL_Renderer* gRenderer,SDL_Window* gWindow);
  void zoomShift(float factor);
};

#endif

#ifndef NECROLAYER_H
#define NECROLAYER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "necroTile.h"


class necroLayer
{
  std::vector<necroTile*>* tiles;
  SDL_Texture* backgroundImage;
  bool hidden;
 public:
  SDL_Rect* layerRect;
  float sceneCoords[2];
  float sceneSize[2];
  int pixelOffest[2];
  int pzelSize[2];
  necroLayer();
  ~necroLayer();
  void renderLayer(SDL_Renderer* gRenderer,SDL_Window* gWindow);
  void updateZoomShift(float zoomFactor, float shiftValue[2]);
};

#endif

#ifndef NECROTILE_H
#define NECROTILE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>


class necroTile{  
  std::vector<necroTile*>* connectedTiles;
  std::vector<int>* connections;
  std::vector<float>* connectionLengths;
  SDL_Texture* tileFrame;
 public:
  SDL_Rect* tileRect; //The rendering rectangle - will need to be shifted/resized as scene is
  float sceneCoords[2]; //Coordinates in scene, shouldn't change much
  float sceneSize[2]; //extent in the scene
  int pixelOffest[2]; //To help reposition the tile
  int pixelSize[2]; //the width and height in pixels of the thing
  enum connectionTypes{
    NC_DIRECT,
    NC_FALL,
    NC_CONDITIONAL_TRUE,
    NC_CONDITIONAL_FALSE
  }
  necroTile(SDL_Texture* frameText);
  ~necroTile();
  void renderTile(SDL_Renderer* gRenderer,SDL_Window* gWindow);
  void updateZoomShift(float zoomFactor, float shiftValue[2]);
};
  

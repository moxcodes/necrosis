#include "necroScene.h"

necroScene::necroScene()
{
  layers=new std::vector<necroLayer * >();
  tileText=new std::vector<SDL_Texture*>();
}

necroScene::~necroScene()
{
  for(std::vector<necroLayer*>::iterator it = layers->begin();it != layers->end(); ++it)
    delete *(*it);
  delete layers;
  
}

void necroScene::renderScene(SDL_Renderer* gRenderer,SDL_Window* gWindow)
{
  for(std::vector<necroLayer >::iterator it = layers->begin();it != layers->end(); ++it)
    (*it)->renderLayer(gRenderer,gWindow);
  return;
}

void necroScene::zoomShift(float factor,float value[2])
{
  zoomFactor=zoomFactor*factor;
  shiftValue[0] += value[0];
  shiftValue[1] += value[1];
  for(std::vector<necroLayer >::iterator it = layers->begin();it != layers->end(); ++it)
    (*it)->updateZoomShift(zoomFactor,shiftValue);
}


#include "necroLayer.h"

necroLayer::necroLayer()
{
  tiles=new std::vector<necroTile *>();
  hidden=false;
}

necroLayer::~necroLayer()
{
  for(std::vector<necroTile * >::iterator it = tiles->begin();it != tiles->end(); ++it)
    delete *(*it);
  delete tiles;
}

void necroLayer::renderLayer(SDL_Renderer* gRenderer,SDL_Window* gWindow)
{
  if(!hidden && backgroundImage!=NULL)
    SDL_RenderCopy(gRenderer, backgroundImage, NULL, layerRect);
  
  for(std::vector<necroTile >::iterator it = tiles->begin();it != tiles->end(); ++it)
    (*it)->renderTile(gRenderer,gWindow);
  return;
}

void necroLayer::updateZoomShift(float zoomFactor, float shiftValue[2])
{
  layerRect->x = sceneCoords[0]*zoomFactor + shiftValue[0];
  layerRect->y = sceneCoords[1]*zoomFactor + shiftValue[1];
  layerRect->w = sceneSize[0]*zoomFactor;
  layerRect->h = sceneSize[1]*zoomFactor;
  for(std::vector<necroTile >::iterator it = tiles->begin();it != tiles->end(); ++it)
    (*it)->updateZoomShift(gRenderer,gWindow);
  return;
}

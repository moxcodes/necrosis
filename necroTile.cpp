#include "necroTile.h"


necroTile::necroTile(SDL_Texture* frameText)
{
  connectedTiles = new std::vector<necroTile *>();
  connections = new std::vector<int>();
  connectionLengths = new std::vector<float>();
  tileFrame=frameText;
}

necroTile::~necroTile()
{
  delete connectedTiles;
  delete connections;
  delete connectionLengths;
  delete tileRect;
}

void necroTile::renderTile()
{
  SDL_RenderCopy(gRenderer,tileFrame,NULL,tileRect);
}

void necroTile::updateZoomShift(float zoomFactor, float ShiftValue[2])
{
  tileRect->x = sceneCoords[0]*zoomFactor + shiftValue[0];
  tileRect->y = sceneCoords[1]*zoomFactor + shiftValue[1];
  tileRect->w = sceneSize[0]*zoomFactor;
  tileRect->h = sceneSize[1]*zoomFactor;
}

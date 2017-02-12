#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "necroScene.h"


necroScene editScene;

enum editorMode
  {
    NE_DEFAULTMODE,
    NE_PANMODE,
    NE_ZOOMMODE,
    NE_TILEDRAWMODE,
    NE_TILEIMPORTMODE
  };

editorMode mode=NE_DEFAULTMODE; 

//Things that will probably be put together into a temporary draw texture class

SDL_Texture *newTile = NULL;
SDL_Renderer *drawRenderer = NULL;
SDL_Color White = {255,255,255};

int tempLineStart[2] = {0,0};
int drawCursorPosition[2] = {0,0};

//Things that will probably be put together into a text input class

char* fileText;
int fileCursor;
int fileLength;

////Player stuff


SDL_Texture* load_texture(std::string filename);


SDL_Texture* load_texture(std::string filename)
{
  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load( filename.c_str() );
  if( loadedSurface == NULL)
    {
      printf( "oopsidaisies! %s didn't load! SDL error: %s\n",filename.c_str(), IMG_GetError());
      return NULL;
    }
  newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
  if( newTexture == NULL )
    printf("balls balls balls %s\n",SDL_GetError() );
  SDL_FreeSurface(loadedSurface);
  return newTexture;
}

void handle_drawing(SDL_Event* event, bool* quit)
{
  if(event->type == SDL_MouseButtonEvent && event->button == SDL_BUTTON_LEFT)
    {
      if(tempLineStart[0]==0&&tempLineStart[1]==0)
	{
	  tempLineStart[0] = event->x;
	  tempLineStart[1] = event->y;
	  drawCursorPosition[0] = event->x;
	  drawCursorPosition[0] = event->y;
	}
      else
	{
	  SDL_RenderDrawLine(drawRenderer,tempLineStart[0]-300,tempLineStart[1]-250,
			     event->x-300,event->y-250);
	  tempLineStart = {0,0};
	  drawCursorPosition = {0,0};
	}
    }
  if(event->type == SDL_MouseMotionEvent && tempLineStart[0] ==0 && tempLineStart[0] ==0)
    {
      drawCursorPosition[0] = event->x;
      drawCursorPosition[0] = event->y;
    }
}


void handle_input(SDL_Event* event, bool* quit)
{
  switch(mode)
    {
    case NE_DEFAULTMODE:
      if(event->type == SDL_KEYDOWN key.keysym.sym && event->key.keysym.sym == SDLK_n)
	mode=NE_TILEDRAWMODE;
      break;
    case NE_PANMODE:
      break;
    case NE_ZOOMMODE:
      break;
    case NE_TILEDRAWMODE:
      if(event->type == SDL_KEYDOWN key.keysym.sym && event->key.keysym.sym == SDLK_n)
	{
	  mode=NE_TILEIMPORTMODE;
	  SDL_StartTextInput();
	  fileText=strdup("");
	  compositionText=strdup("");
	  newTile=NULL;
	}
      handle_drawing(event, quit)
      break;
    case NE_TILEIMPORTMODE:
      switch (event.type)
	{
	case SDL_QUIT:
	  /* Quit */
	  done = SDL_TRUE;
	  break;
	case SDL_TEXTINPUT:
	  /* Add new text onto the end of our text */
	  strcat(fileText, event.text.text);
	  break;
	case SDL_TEXTEDITING:
	  /*
	    Update the composition text.
	    Update the cursor position.
	    Update the selection length (if any).
	  */
	  fileText = event.edit.text;
	  fileCursor = event.edit.start;
	  fileLength = event.edit.length;
	  break;
      }
      break;
      if(fileText[fileLength-1]=='\n')
	{
	// handle importing the tile, adding it to the scene etc
	  mode=NE_DEFAULTMODE;
	}
    }
  return;
}

int main (int argc, char* args[])
{
  int SCREEN_WIDTH = 800;
  int SCREEN_HEIGHT = 600;
  int SCREEN_BPP = 32;

  SDL_Surface* image = NULL;
  SDL_Surface* screen = NULL;

  SDL_Window* gWindow = NULL;
  SDL_Renderer* gRenderer = NULL;
  SDL_Texture* gTexture = NULL;

  //main setup phase
  TTF_Init();
  SDL_Init( SDL_INIT_EVERYTHING );
  SDL_StopTextInput();
  gWindow = SDL_CreateWindow( "necrosis", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
  drawRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
  
  SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF );
  IMG_Init( IMG_INIT_PNG );

  

  SDL_Texture* newTileFrame = load_texture( "img/new_tile.png" );
  SDL_Rect newTileRect;
  newTileRect.w=200;newTileRect.h=100;newTileRect.x=300;newTileRect.y=250;

  
  //  image = load_image("image.jpg");
  TTF_Font* Sans = TTF_OpenFont("fonts/Inconsolata-Bold.ttf",24);

  bool quit = false;

  SDL_Event event;
  // The main loop - this will go until the game is shutting down.
  int loopDuration=10;
  int prevTicks=SDL_GetTicks();
  while(!quit)
    {
      
      while( SDL_PollEvent( &event ) != 0 )
	{
	  if( event.type == SDL_QUIT )
	    {
	      quit = true;
	    }
	}

      handle_keys(&event,&quit);
      editScene.renderScene(gRenderer,gWindow);

      if(mode==NE_TILEDRAWMODE)
	{
	  SDL_RenderCopy(gRenderer,newTileFrame,NULL,&newTileRect);
	  if(newTile==NULL)
	    {
	      *newTile = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 200, 100);
	      SDL_SetRenderTarget(drawRenderer,newTile);
	    }
	  SDL_RenderCopy(gRenderer,newTile,NULL,&newTileRect);
	}
      
      loopDuration=SDL_GetTicks()-prevTicks;
      prevTicks=SDL_GetTicks();
    }


  SDL_DestroyTexture( gTexture );
  SDL_DestroyTexture( gridTexture );
  SDL_DestroyTexture( selectTexture );
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;
  IMG_Quit();
  SDL_Quit();

  return 0;
}

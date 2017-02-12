#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int SCREEN_BPP = 32;

SDL_Surface* image = NULL;
SDL_Surface* screen = NULL;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;


/////Cursor and map stuff:
SDL_Rect cursorRect;

bool cursorDeactivate=false;
int cursorCooldown=0;

int protomap[5][5] =
  {{1,1,1,1,1},
   {1,1,1,1,1},
   {1,1,1,1,0},
   {1,1,1,1,1},
   {1,1,1,1,1}};

float cursorCoords[2] = {0,0};
/////

////Player stuff
bool moveMode=false;
int moveModeCooldown=0;
float maxPlayerMovement=2.1;
std::vector<std::vector<int> > playerMoveOptions;
SDL_Texture* playerTexture;
float playerCoords[2] = {2,4};
float newPlayerCoords[2] = {2,4};
int turn=0;


SDL_Texture* load_texture(std::string filename);

void computeMoveOptions(float coords[2],float maxMovement,
			std::vector<std::vector<int> >* moveOptions);

bool isMoveOption(float coords[2],std::vector<std::vector<int> >* moveOptions);

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

bool isMoveOption(float coords[2],std::vector<std::vector<int> >* moveOptions)
{
  for (std::vector<std::vector<int> >::iterator it = playerMoveOptions.begin();
       it != playerMoveOptions.end(); ++it)
    {
      if((*it).at(0)==coords[0] && (*it).at(1)==coords[1])
	return true;
    }
  return false;
}


void handle_keys(SDL_Event* event, bool* quit)
{
  bool noKeyDown=true;
  if(event->type == SDL_KEYDOWN)
    {
      switch( event->key.keysym.sym )
	{
	case SDLK_ESCAPE:
	  *quit = true;
	case SDLK_SPACE:
	  if(moveMode && isMoveOption(cursorCoords,&playerMoveOptions)&&
	     playerCoords[0]==newPlayerCoords[0]&&playerCoords[1]==newPlayerCoords[1])
	    {
	      newPlayerCoords[0] = cursorCoords[0];
	      newPlayerCoords[1] = cursorCoords[1];
	      turn++;
	    }
	  break;
	case SDLK_m:
	  if(moveModeCooldown<SDL_GetTicks())
	    {
	      moveMode=!moveMode;
	      moveModeCooldown = SDL_GetTicks() + 200;
	    }
	  break;
	case SDLK_LEFT:
	  noKeyDown=false;
	  if(cursorCoords[0]-1 >=0 && protomap[(int)cursorCoords[1]][(int)cursorCoords[0]-1]==1
	     &&!cursorDeactivate)
	    {
	      cursorCoords[0]=cursorCoords[0]-1;
	      cursorDeactivate=true;
	      cursorCooldown=SDL_GetTicks()+150;
	    }
	  break;
	case SDLK_RIGHT:
	  noKeyDown=false;
	  if(cursorCoords[0]+1 <5 && protomap[(int)cursorCoords[1]][(int)cursorCoords[0]+1]==1
	     &&!cursorDeactivate)
	    {
	      cursorCoords[0]=cursorCoords[0]+1;
	      cursorDeactivate=true;
	      cursorCooldown=SDL_GetTicks()+150;
	    }
	  break;
	case SDLK_UP:
	  noKeyDown=false;
	  if(cursorCoords[1]-1 >=0 && protomap[(int)cursorCoords[1]-1][(int)cursorCoords[0]]==1
	     &&!cursorDeactivate)
	    {
	      cursorCoords[1]=cursorCoords[1]-1;
	      cursorDeactivate=true;
	      cursorCooldown=SDL_GetTicks()+150;
	    }
	  break;
	case SDLK_DOWN:
	  noKeyDown=false;
	  if(cursorCoords[1]+1 <5 && protomap[(int)cursorCoords[1]+1][(int)cursorCoords[0]]==1
	     &&!cursorDeactivate)
	    {
	      cursorCoords[1]=cursorCoords[1]+1;
	      cursorDeactivate=true;
	      cursorCooldown=SDL_GetTicks()+150;
	    }
	  break;
	}
      if(noKeyDown){
	cursorCooldown=0;
	cursorDeactivate=false;
      }
    }	
  return;
}


void computeMoveOptions(float coords[2],float maxMovement,
			std::vector<std::vector<int> >* moveOptions)
{
  moveOptions->erase(moveOptions->begin(),moveOptions->end());
  for(int i=0; i<5;i++)
    {
      for(int j=0;j<5;j++)
	{
	  if(pow(pow((i-coords[0]),2) + pow((j - coords[1]),2),.5) <= maxMovement
	     && !(i==coords[0]&&j==coords[1]) && protomap[j][i]==1)
	    {
	      std::vector<int> coordAdd = {i,j};
	      moveOptions->push_back(coordAdd);
	    }
	}
    }
   return;
}

int main (int argc, char* args[])
{
  //main setup phase
  TTF_Init();
  SDL_Init( SDL_INIT_EVERYTHING );
  SDL_StopTextInput();
  gWindow = SDL_CreateWindow( "necrosis", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
  if( gWindow == NULL)
    {
      printf("well. Fuck. Sorry boys");
      return -1;
    }

  gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );

  if( gRenderer == NULL )
    {
      printf("okay, no hardware acceleration. This one is on you, pal.");
      return -1;
    }

  SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF );
  int imgFlags = IMG_INIT_PNG;
  IMG_Init( imgFlags );


  SDL_Texture* gridTexture = load_texture( "img/necrosis_prototype_bg.png" );
  SDL_Texture* selectTexture = load_texture( "img/necrosis_prototype_select.png" );

  SDL_Texture* moveTexture = load_texture( "img/necrosis_prototype_move.png" );
  
  playerTexture = load_texture( "img/player_coin.png" );
  cursorRect.w=146;
  cursorRect.h=85;
  cursorRect.x=37;
  cursorRect.y=119;
  SDL_Rect moveRect;
  moveRect.w=146;
  moveRect.h=85;
  
  //  image = load_image("image.jpg");
  TTF_Font* Sans = TTF_OpenFont("fonts/Inconsolata-Bold.ttf",24);
  SDL_Color White = {255,255,255};
  SDL_Surface* turnSurface; 
  SDL_Texture* turnTexture;
  SDL_Rect turnRect;
  turnRect.x=20;
  turnRect.y=500;
  SDL_Rect playerRect;
  playerRect.w=90;
  playerRect.h=60;
  
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
      std::string text="turn " + std::to_string(turn);
      
      turnSurface = TTF_RenderText_Solid(Sans,text.c_str(),White);
      turnRect.w=turnSurface->w;
      turnRect.h=turnSurface->h;
      turnTexture = SDL_CreateTextureFromSurface(gRenderer, turnSurface);

      //      SDL_FreeSurface(turnSurface);
      computeMoveOptions(playerCoords,maxPlayerMovement,&playerMoveOptions);

      

      handle_keys(&event,&quit);
      if(cursorCooldown<SDL_GetTicks())
	cursorDeactivate=false;
      cursorRect.x=cursorCoords[0]*100.5+37 + cursorCoords[1]*54.5;
      cursorRect.y=cursorCoords[1]*54.5+119;

      if(newPlayerCoords[0]!=playerCoords[0]||newPlayerCoords[1]!=playerCoords[1])
	{
	  playerCoords[0]+=.001*loopDuration*((newPlayerCoords[0]-playerCoords[0])
	    /(pow(pow(newPlayerCoords[0]-playerCoords[0],2)
		  + pow(newPlayerCoords[1]-playerCoords[1],2),.5)));
	  playerCoords[1]+=.001*loopDuration*((newPlayerCoords[1]-playerCoords[1])
	    /(pow(pow(newPlayerCoords[0]-playerCoords[0],2)
		  + pow(newPlayerCoords[1]-playerCoords[1],2),.5)));
	}
      
      if(fabs(playerCoords[0]-newPlayerCoords[0]) <=.001*(float)loopDuration
	 && fabs(playerCoords[1]-newPlayerCoords[1]) <=.001*(float)loopDuration )
      {
        playerCoords[0]=newPlayerCoords[0];
        playerCoords[1]=newPlayerCoords[1];
      }
      playerRect.x=playerCoords[0]*100.5+65 + playerCoords[1]*54.5;
      playerRect.y=playerCoords[1]*54.5+125;



      SDL_RenderClear( gRenderer );
      SDL_RenderCopy( gRenderer, gridTexture, NULL, NULL );
      SDL_RenderCopy( gRenderer, turnTexture, NULL, &turnRect );
      if(moveMode)
	{
	  for (std::vector<std::vector<int> >::iterator it = playerMoveOptions.begin();
	       it != playerMoveOptions.end(); ++it)
	    {
	      moveRect.x=(*it).at(0)*100.5+37 + (*it).at(1)*54.5;
	      moveRect.y=(*it).at(1)*54.5+119;
	      SDL_RenderCopy( gRenderer, moveTexture, NULL, &moveRect );
	    }
	}
      SDL_RenderCopy( gRenderer, selectTexture, NULL, &cursorRect );
      SDL_RenderCopy( gRenderer, playerTexture, NULL, &playerRect );
      SDL_RenderPresent( gRenderer );
      //      SDL_DestroyTexture(turnTexture);
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

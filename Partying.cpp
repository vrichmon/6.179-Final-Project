#include <SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cmath>

//good place to look for general help (how I got this far): http://lazyfoo.net/tutorials/SDL/index.php 

//Screen dimension constants (you can change)
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 420;
int numBalls=10;

//Starts up SDL and creates window, apparently
bool init();

//??
bool loadMedia();
void close();
SDL_Texture* loadTexture( std::string path );

//The window to "render" to
SDL_Window* gWindow = NULL;

//The renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL couldn't' initialize... SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//texture stuff
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning...texture issues" );
		}

		//makes window
		gWindow = SDL_CreateWindow( "Ball Game!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window couldn't be created.. :( SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "the renderer couldn't be created... :((( SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				// renderer color (blue right now)
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//for pictures
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "image had issues... SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

//copied from online... don't think this is ever used
bool loadMedia()
{
	bool success = true;

	return success;
}

void close()
{
	//close window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit everything!
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	SDL_Texture* newTexture = NULL;

	//for pics
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Image load issues for: %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//"texture" stuff
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "exture issues from: %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

//Actual graphics stuff contained here
int main( int argc, char* args[] )
{
	//start everything, make a window
	if( !init() )
	{
		printf( "couldn't initialize\n" );
	}
	else
	{
		if( !loadMedia() )
		{
			printf( "couldn't 'load media'\n" );
		}
		else
		{	
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While window should be open + running
			while( !quit )
			{
				//Handle events
				while( SDL_PollEvent( &e ) != 0 )
				{
					//close window
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

//
//				//Render green outlined quad
//				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
//				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
//				SDL_RenderDrawRect( gRenderer, &outlineRect );
				
				//Blue horizontal line, for scorekeeping box(?)
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
				SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 10, SCREEN_WIDTH/8, SCREEN_HEIGHT / 10 );

				//vertical line for scorekeeping box (?)
				for( int i = 0; i < SCREEN_HEIGHT/10; i++ )
				{
					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 8, i );
				}

				//draw a ball of random color
				int num;
				int temp;
				for (num=0; num<numBalls; num++) {
					
					//get random color using rand() and switch statements
					temp = rand() % 9; //num between 0 and 8
					switch(temp){
					   case 0 :
					      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF ); // pink
					      break;
					   case 1 :
					   		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF ); //blue
							break;
					   case 2 :
						  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xA5, 0x00, 0xFF );  //orange
					      break;
					   case 3 :
					      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF ); //red?
					      break;
					   case 4:
						  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF ); //yellow?
					      break;
					    case 5 :
					      SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF ); //purple?
					      break;
					   case 6:
						  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF ); //??
					      break;
					   case 7 :
					      SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0xFF, 0xFF ); //?
					      break;
					   case 8 :
					      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF ); //?
					      break;
					   default :
						  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF  ); //pinkkk
						}
						
						
						//draw circle in middle
					   	double R = 20;
					    double Deg0 = 0;
						double Deg90 = M_PI / 2;
						double DegStep = Deg90 / (R * 4);
						double CurrDeg = Deg0;
						double OffsetX = R;
						double OffsetY = 0;
						double TmpR = R;
						double CX = rand()%SCREEN_WIDTH;//SCREEN_WIDTH/2;
						double CY = rand()%SCREEN_HEIGHT;//SCREEN_HEIGHT/2;
						while(TmpR>0 )
						{
						    while(CurrDeg < Deg90)
						    {
						        OffsetX = cos(CurrDeg) * TmpR;
						        OffsetY = sin(CurrDeg) * TmpR;
						        SDL_RenderDrawPoint(gRenderer, CX+(int)OffsetX, CY+(int)OffsetY);
						        SDL_RenderDrawPoint(gRenderer, CX-(int)OffsetY, CY+(int)OffsetX);
						        SDL_RenderDrawPoint(gRenderer, CX-(int)OffsetX, CY-(int)OffsetY);
						        SDL_RenderDrawPoint(gRenderer, CX+(int)OffsetY, CY-(int)OffsetX);
						        CurrDeg+=DegStep;
						    }
						    CurrDeg = Deg0;
						    TmpR-=1;
						}				
				}
				
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//done
	close();

	return 0;
}

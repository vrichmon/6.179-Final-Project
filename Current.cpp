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
int numBalls=30;
int time = 0;

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


typedef struct ball_t {
	double dx; //speed defined like a vector, with x+y components
	double dy;
	int color; //0-7 (actual color determined by switch statement)
	double currX;
	double currY;
} Ball;


Ball *array = new Ball[numBalls]; //array of pointers to all the balls


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

void erase() {
	int i;
	for(i=0; i<numBalls;i++){
		
		delete(&array[i]);
		
	}
	
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
			while( !quit ){
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

				if (time==0) {
					time =1;
					
					//create balls of random color and put it in array
					int num;
					double tempx=0;
					double tempy=0;
					int temp, neg;
					for (num=0; num<numBalls; num++) {
						
						//get random color using rand() and switch statements
						temp = rand() % 9; //num between 0 and 8
						
						Ball *newOne = new Ball;
						newOne->currX=rand()%SCREEN_WIDTH;
						newOne->currY=rand()%SCREEN_HEIGHT;
						newOne->color=temp;
						
						//set random dx and dy values
						while (tempx==0 && tempy==0) { //don't want both to be 0s, so if you get randomly 2 0s, redo until don't
							//find ranodm dx value, pos or neg
							neg = rand()%2; //0 or 1
							if (neg==1){
								neg=1;
							} else{
								neg=-1;
							}
							tempx= neg* (rand()%6) * 0.1 ;
						
							//find random dy value, pos or neg
							neg = rand()%2; //0 or 1
							if (neg==1){
								neg=1;
							} else{
								neg=-1;
							}
							tempy= neg* (rand()%6) * 0.1 ; //between -.5 and .5
						}
						newOne->dx= tempx;
						newOne->dy= tempy;
						
						
						//put this new ball into array
						array[num] = *newOne ;//address of the ball populates the array
					}
					
					//now we have the array of pointers to all the balls... now we should do something with it (display them)
					
					int k;
					for (k=0; k<numBalls; k++){
						
						switch(array[k].color){
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
							
							
						//draw circle
					   	double R = 20;
					    double Deg0 = 0;  //degree stuff so that we can draw a circle using only the drw pixel command..
						double Deg90 = M_PI / 2;
						double DegStep = Deg90 / (R * 4);
						double CurrDeg = Deg0;
						double OffsetX = R;
						double OffsetY = 0;
						double TmpR = R;
						double CX = array[k].currX;//SCREEN_WIDTH/2; //middle
						double CY = array[k].currY;//SCREEN_HEIGHT/2;
						while(TmpR>0 ){
						    while(CurrDeg < Deg90){
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
					
				} //<--first time only if statement ends
					
				if (time ==1){ //if not the first time
				
					
					SDL_RenderClear(gRenderer); //clear the screen
					
					int y;
					for (y=0; y<numBalls; y++) { 
					
						//draw circle
					   	double R = 20;
					    double Deg0 = 0;  //degree stuff so that we can draw a circle using only the draw pixel command..
						double Deg90 = M_PI / 2;
						double DegStep = Deg90 / (R * 4);
						double CurrDeg = Deg0;
						double OffsetX = R;
						double OffsetY = 0;
						double TmpR = R;
						
						//HANDLE BOUNCES
						
						array[y].currX = array[y].currX+ array[y].dx; //update the position based on the speed
						array[y].currY = array[y].currY+ array[y].dy; 
						double CX = array[y].currX;//SCREEN_WIDTH/2; //middle
						double CY = array[y].currY;//SCREEN_HEIGHT/2;
						while(TmpR>0 ){
						    while(CurrDeg < Deg90){
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
					
					}//<--for each ball
					
				} //<--end of not first time loop
				
				SDL_RenderPresent( gRenderer ); //update screen
				
			}//<--end of "while(!quit) loop"
		
			erase();
			close();
				
		}
			
	}
		
	return 0;
}

#include <SDL.h>
#include <SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <unistd.h>
//#include <SDL_ttf.h>

//global variables 
const int SCREEN_WIDTH = 600;//Screen dimension constants (subject to change)
const int SCREEN_HEIGHT = 420;
int numBalls=50;
int MAX_numBalls=50;
int time = 0;
double R = 20; //radius
int points=0;
double clickX=-1;
double clickY=-1;
Mix_Chunk *blop;
Mix_Chunk *cheer;
bool clicked = false;
bool levelUp=false;
bool over=false;
typedef SDL_Texture* Texture;
Texture background_image;
Texture start_image;
bool started=false;
int levelNum = 2;//one, but 2 because index starts at 0

//Starts up SDL and creates window, apparently
bool init();

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
//Ball *clicked = new Ball[numBalls];

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
		gWindow = SDL_CreateWindow( "Bubble Game!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

//never used
bool loadMedia()
{
	
//	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size
//
//	SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
//	
//	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
//	
//	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
//	
//	SDL_Rect Message_rect; //create a rect
//	Message_rect.x = 0;  //controls the rect's x coordinate 
//	Message_rect.y = 0; // controls the rect's y coordinte
//	Message_rect.w = 100; // controls the width of the rect
//	Message_rect.h = 100; // controls the height of the rect
//	
//	//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance
//	
//	//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes
//	
//	SDL_RenderCopy(gRenderer, "Score", NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
		
	bool success = true;

	return success;
}

void erase() {
	int i;
	for(i=0; i<MAX_numBalls;i++){
		
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
	exit(1);
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
			printf( "texture issues from: %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

drawCircle(double x, double y){
	
    double Deg0 = 0;  //degree stuff so that we can draw a circle using only the drw pixel command..
	double Deg90 = M_PI / 2;
	double DegStep = Deg90 / (R * 4);
	double CurrDeg = Deg0;
	double OffsetX = R;
	double OffsetY = 0;
	double TmpR = R;
	double CX = x;//SCREEN_WIDTH/2; //middle
	double CY = y;//SCREEN_HEIGHT/2;
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

void drawOutline() {
	
	double Deg0 = 0;  //degree stuff so that we can draw a circle using only the drw pixel command..
	double Deg90 = M_PI / 2;
	double DegStep = Deg90 / (5*R * 4);
	double CurrDeg = Deg0;
	double OffsetX = R;
	double OffsetY = 0;
	double TmpR = 5*R+.5*R;
	double CX = clickX;
	double CY = clickY;
	while(TmpR>16*R/3 ){
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

setColor(int color){
	
	switch(color){
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
		  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF ); //??
	      break;
	   case 7 :
	      SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0xFF, 0xFF ); //?
	      break;
	   case 8 :
	      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF ); //?
	      break;
	   case 9:
	   	  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF); //WHITE
	   	  break;
	   default :
		  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF  ); //pinkkk
	}
	
}

bool onB(double x, double y){
	
	//hitting the floor (bottom)
	if (y>(SCREEN_HEIGHT-R)){
		return true;
	}
	return false;
	
}

bool onT(double x, double y){
	
	//hitting the cieling (top)
	if (y<R){
		return true;
	}
	return false;
	
}

bool onL(double x, double y){
	
	//hitting the left wall
	if ( x<R ){
		return true;
	}
	return false;
	
}

bool onR(double x, double y){
	
	//hitting the right wall
	if ( x>(SCREEN_WIDTH-R) ){
		return true;
	}
	return false;
	
}

void update(int y){
	
	array[y].currX = array[y].currX+ array[y].dx; //update the position based on the speed
	array[y].currY = array[y].currY+ array[y].dy; 
	
}

double dist(double x1, double y1, double x2, double y2){
	return sqrt(pow((x1-x2),2)+ pow((y1-y2),2));
}

Texture loadImage(char* image){
   SDL_Surface *loadedImage = IMG_Load(image);
   if(!loadedImage) {
      printf("Failed to load image: %s\n", SDL_GetError() );
      SDL_Quit();
      exit(1);
   }
   Texture texture = SDL_CreateTextureFromSurface(gRenderer, loadedImage);
   SDL_FreeSurface(loadedImage);
   if(!texture) {
      printf("Failed to create texture: %s\n", SDL_GetError() );
      SDL_Quit();
      exit(1);
   }
   return texture;
}

void displayTexture(Texture t, unsigned x, unsigned y, unsigned width, unsigned height, SDL_RendererFlip flip){
   SDL_Rect tex_size;
   tex_size.x = 0;
   tex_size.y = 0;
   SDL_Rect toplace;
   toplace.x = x;
   toplace.y = y;
   toplace.w = width;
   toplace.h = height;
   SDL_QueryTexture(t, NULL, NULL, &tex_size.w, &tex_size.h);
   SDL_RenderCopyEx(gRenderer,t,&tex_size,&toplace,0,NULL,flip);
}

void endScreen(){
	
	over=true;
	
	//make a final score screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
	SDL_RenderClear( gRenderer );
	
	background_image = loadImage("end.png");
	displayTexture(background_image, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_FLIP_NONE);
	
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF );

	//point tallies
	int thickness=5;
	int j,i,k;		
	for (j=0; j<points;j++){
		for( i = SCREEN_HEIGHT/3.5; i < 13*SCREEN_HEIGHT/14; i++ )
		{
			for (k=0;k<thickness;k++){
				SDL_RenderDrawPoint( gRenderer, (j+1)*(SCREEN_WIDTH)/(points)+k, i );
			}
		}
//		SDL_RenderDrawLine( gRenderer, 0,10,20,10);
	}
			
	SDL_RenderPresent( gRenderer ); //update screen	
	
}

void level(){
		
//	printf("level called");
	//update all values
	if (numBalls>10){
		levelNum++;
		numBalls=numBalls-10;
		Mix_PlayChannel( -1, cheer, 0 );
		clicked= false;
		clickX=-1;
		clickY=-1;
		time=0;
	} else {
		endScreen();
	}
	
	levelUp=false;
	
}

void onStart(){
	started=true;	//could put in for call..
}

void onClick(double x, double y){
	
	if (!clicked) {
		
		clicked=true;
		
		Mix_PlayChannel( -1, blop, 0 ); //sound
		
		clickX=x;
		clickY=y;
		
		int i;
		for (i=0; i<numBalls;i++){ //for every ball
			
			//if distance to click is less than radius, add it to clicked array 
			if(dist(x,y,array[i].currX, array[i].currY)<5*R){
				array[i].dx=0;
				array[i].dy=0;
				points++;
			} else {
				array[i].dx=0;
				array[i].dy=0;
				array[i].currX=SCREEN_WIDTH+R;
				array[i].currY=SCREEN_HEIGHT+R;
				array[i].color = 9;
			}
			
		}
		
		levelUp=true;
		
//		printf("points: %d\n", points);
			
	}
	
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
		
			if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
		        printf("Audio creation failed %s\n", Mix_GetError());
		        return 1;    
		    }
		
		    blop = Mix_LoadWAV("blop.wav");
		    if(!blop){
		        printf("Could not open sound effect %s\n", Mix_GetError());
		    }	
		    
		    cheer = Mix_LoadWAV("cheer.wav");
		    if(!cheer){
		        printf("Could not open sound effect %s\n", Mix_GetError());
		    }	
		    
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While window should be open + running
			while( !quit ){
				
				if (!started){
					
					start_image = loadImage("start.png");
					displayTexture(start_image, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_FLIP_NONE);
					SDL_RenderPresent( gRenderer ); //update screen
					
					while (SDL_PollEvent(&e)){
			        	 if (e.type == SDL_QUIT){
			         		 quit = 1;
			    	 	} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				            onStart();
	//		    	 		quit=1; //comment out (for testing)--working
			         	}
			      	}
					
					//Handle events
					while( SDL_PollEvent( &e ) != 0 )
					{
						//close window
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}
					}
					//do stuff
					
				} else {
				
					while (SDL_PollEvent(&e)){
			        	 if (e.type == SDL_QUIT){
			         		 quit = 1;
			    	 	} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				            onClick(e.button.x, e.button.y);
	//		    	 		quit=1; //comment out (for testing)--working
			         	}
			      	}
					
					//Handle events
					while( SDL_PollEvent( &e ) != 0 )
					{
						//close window
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}
					}
					
					if (!over) {
		
						//Clear screen
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
						SDL_RenderClear( gRenderer );
						
						//Blue horizontal line, for scorekeeping box
						SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
						SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 10, SCREEN_WIDTH/8, SCREEN_HEIGHT / 10 );
						
						//vertical line for scorekeeping box
						for( int i = 0; i < SCREEN_HEIGHT/10; i++ )
						{
							SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 8, i );
						}		
						
						//point tallies	
						int j;		
						for (j=0; j<points;j++){
							int h;
							for( h = SCREEN_HEIGHT/70; h < 6*SCREEN_HEIGHT/70; h++ )
							{
								SDL_RenderDrawPoint( gRenderer, (j+1)*(SCREEN_WIDTH / 8)/(points), h );
							}
	//						SDL_RenderDrawLine( gRenderer, 0,10,20,10);
						}
						
						// horizontal line, for level# box
						SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0xFF );		
						SDL_RenderDrawLine( gRenderer, 7*SCREEN_WIDTH/8, 9*SCREEN_HEIGHT / 10, SCREEN_WIDTH, 9*SCREEN_HEIGHT / 10 );
						
						//vertical line for level# box
						for( int i = SCREEN_HEIGHT; i> 9*SCREEN_HEIGHT/10; i-- ) 
						{
							SDL_RenderDrawPoint( gRenderer, 7*SCREEN_WIDTH / 8, i );
						}	
						
						//level tallies			
						for (j=0; j<levelNum;j++){
							int h; //wasn't in this scope earlier
							for( h = 69*SCREEN_HEIGHT/70; h > 64*SCREEN_HEIGHT/70; h-- )
							{
								SDL_RenderDrawPoint( gRenderer, 7*SCREEN_WIDTH/8+(j+1)*(SCREEN_WIDTH / 8)/(levelNum), h );
							}
	//						SDL_RenderDrawLine( gRenderer, 0,10,20,10);
						}						
						
						if(clickX!=-1 && clickY!=-1){ //draw click radius
							SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF ); //black
							drawOutline();
						} 
				
						if (time==0) {
							time =1;
							
							//create balls of random color and put it in array
							int num;
							double tempx;
							double tempy;
							int temp, neg;
							for (num=0; num<numBalls; num++) {
								
								tempx=0;
							    tempy=0;
								
								//get random color using rand() and switch statements
								temp = rand() % 9; //num between 0 and 8
								
								Ball *newOne = new Ball;
								newOne->currX=rand()%SCREEN_WIDTH;
								newOne->currY=rand()%SCREEN_HEIGHT;
								newOne->color=temp;
								
								//set random dx and dy values
								while (tempx==0 && tempy==0) { //don't want both to be 0s, so if you get randomly 2 0s, redo until don't
								
		//							printf("loop to find speeds--should show up 50 times");//WORKING
									//find ranodm dx value, pos or neg
									neg = rand()%2; //0 or 1
									if (neg==1){
										neg=1;
									} else{
										neg=-1;
									}
									tempx= neg*( (rand()%20) +4);
								
									//find random dy value, pos or neg
									neg = rand()%2; //0 or 1
									if (neg==1){
										neg=1;
									} else{
										neg=-1;
									}
									tempy= neg*( (rand()%20) +4); 
								}
								newOne->dx= tempx;
								newOne->dy= tempy;
								
								//put this new ball into array
								array[num] = *newOne ;//address of the ball populates the array
							}
							
							//now we have the array of pointers to all the balls... now we should do something with it (display them)
							
							int k;
							for (k=0; k<numBalls; k++){
								
								setColor(array[k].color);
									
								drawCircle(array[k].currX, array[k].currY); //draw circle			
							
							}
							
						} //<--first time only if statement ends
							
						if (time == 1){ //if not the first time
											
							int y;
							for (y=0; y<numBalls; y++) { 
										
								//handling bounces and corner behaviors
								if(onL(array[y].currX, array[y].currY)){ //if hits y (vertical) walls, change the x sign		
									array[y].dx= abs(array[y].dx);	
								} 
								if (onR(array[y].currX, array[y].currY)){ //if hits x (horizontal floors/cieling), change the y sign
									array[y].dx= -1*abs(array[y].dx);
								} 
								if (onT(array[y].currX, array[y].currY)){ //if hits x (horizontal floors/cieling), change the y sign
									array[y].dy= abs(array[y].dy);
								} 
								if (onB(array[y].currX, array[y].currY)){ //if hits x (horizontal floors/cieling), change the y sign
									array[y].dy= -1*abs(array[y].dy);
								}
									
								update(y);
								
								
								setColor(array[y].color);
								
								drawCircle(array[y].currX, array[y].currY); //draw circle
							
							}//<--for each ball
							
						} //<--end of not first time loop
						
						SDL_RenderPresent( gRenderer ); //update screen
						
						if(levelUp){
							clickX=-1;
							clickY=-1;
							sleep(1.2); //pause 1.2 seconds
							level();
						}
					}//<--end of "not over" if statement	
				}
			}//<--end of "while(!quit) loop"
		
			close();
			erase();
				
		}
			
	}
		
	return 0;
}

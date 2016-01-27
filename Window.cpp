#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
typedef SDL_Texture* Texture;

unsigned window_x = 800;
unsigned window_y = 600;

unsigned window_start_x = 200;
unsigned window_start_y = 100;

char window_name[] = "Ball Game!";

double player1 = 0.5;
double player2 = 0.5;
double player_height = .15;
double player_width  = .03;
double player_speed = 0.01;

int player1_up = 0;
int player2_up = 0;
int player1_down = 0;
int player2_down = 0;

SDL_Renderer *renderer;

Texture loadImage(char* image){
   SDL_Surface *loadedImage = IMG_Load(image);
   if(!loadedImage) {
      printf("Failed to load image: %s\n", SDL_GetError() );
      SDL_Quit();
      exit(1);
   }
   Texture texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
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
   SDL_RenderCopyEx(renderer,t,&tex_size,&toplace,0,NULL,flip);
}

Texture background_image, player_image;

int main( int argc, char* argv[] ){
   if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
      printf("SDL_Init failed: %s\n", SDL_GetError());
      return 1;
   }

   SDL_Window *window = SDL_CreateWindow(window_name,window_start_x,window_start_y, window_x,window_y,SDL_WINDOW_SHOWN);
   if (!window){
      printf("Window creation failed: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
   }
   
   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if (!renderer){
      printf("Renderer creation failed: %s\n", SDL_GetError());
      SDL_Quit();
      return 1;
   }

   //Our event structure
   SDL_Event e;
   int quit = 0;
   while (!quit){
      while (SDL_PollEvent(&e)){
         if (e.type == SDL_QUIT) quit = 1;
         else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym){
//               case SDLK_1:
//                  if(e.type == SDL_KEYDOWN) Mix_PlayChannel( -1, laser, 0 ); break;
               case SDLK_UP:
                  player2_down = (e.type == SDL_KEYDOWN); break;
               case SDLK_DOWN:
                  player2_up = (e.type == SDL_KEYDOWN); break;
               case SDLK_w:
                  player1_down = (e.type == SDL_KEYDOWN); break;
               case SDLK_s:
                  player1_up = (e.type == SDL_KEYDOWN); break;
            }
         }
      }
      
      player1+= (player1_up - player1_down) * player_speed;
      if(player1 > 1.0 - player_height) player1 = 1.0 - player_height ;
      else if(player1 < 0) player1 = 0;

      player2+= (player2_up - player2_down) * player_speed;
      if(player2 > 1.0 - player_height ) player2 = 1.0 - player_height;
      else if(player2 < 0) player2 = 0;
     

      SDL_RenderClear(renderer);

      SDL_SetRenderDrawColor(renderer,255,255,255,255);
      SDL_Rect rectangle;
      rectangle.x = 0;
      rectangle.y = 0;
      rectangle.w = window_x;
      rectangle.h = window_y;
      SDL_RenderFillRect(renderer, &rectangle);

      displayTexture(background_image, 200, 200, 200, 200, SDL_FLIP_NONE);
      displayTexture(player_image, 0, (int)((player1)*window_y),
         (int)(player_width*window_x), (int)(player_height*window_y),SDL_FLIP_NONE);
      displayTexture(player_image, window_x-(int)(player_width*window_x), (int)((player2)*window_y),
         (int)(player_width*window_x), (int)(player_height*window_y),SDL_FLIP_HORIZONTAL);
      SDL_RenderPresent(renderer);
   }
   return 0;
}

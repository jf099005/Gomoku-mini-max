#include<ctime>
#include<iostream>
#include<vector>
using namespace std;

#include "SDL2/SDL.h"
#include "show_pictures.h" 
#include "button.h"
#include "start_page.h"
#include "chess_priority_V5.h"
#include "chess_board.h"
 
int main(int argc, char *argv[])
{
	srand(time(NULL));
    SDL_Window *window = NULL; 
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Event event;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }
	 
	int wid=35;
	
    window = SDL_CreateWindow("Gomoku", 0, 50, 800, 600, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
     
	if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
        return 3;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s", SDL_GetError());
        return 3;
    }
		
	//end_page(renderer,1);
	
	pair<int,int> corner={20,20};//{56-wid,56-wid};
	//add_picture(renderer,"./img/board.bmp",corner.first,corner.second,wid*14,wid*14);
	int x=-1,y=-1;
	vector< pair<int,int> > board_chesses;
	
	int game_mode=start_page(renderer);
	
	if(game_mode!=-1){
	//	cout<<"START PLAYGAME\n";
		
		chess_board gameBoard(renderer,wid,game_mode,corner);
		gameBoard.play_game();
		
	}
	
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}


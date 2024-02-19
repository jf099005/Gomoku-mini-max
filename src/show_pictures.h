#include "SDL2/SDL.h" 
#include "SDL2/SDL_image.h" 
//#include "glm/glm.hpp" 
void add_picture(SDL_Renderer* renderer,const char* picture){
	static SDL_Surface *surface=NULL;
	//surface=SDL_LoadBMP("./img/sample.bmp");
	surface=IMG_Load(picture);
	SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_RenderCopy(renderer,texture,NULL,NULL);
//	delete surface;
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return;
}


void add_picture(SDL_Renderer* renderer,const char* picture,int x,int y,int w,int h){
	static SDL_Surface* surface=NULL;
	surface=IMG_Load(picture);
	static SDL_Rect r;
	r.x=x;r.y=y;r.w=w;r.h=h;
	
	SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_RenderCopy(renderer,texture,NULL,&r);
//	delete surface;
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return;
}

void add_picture(SDL_Renderer* renderer,const char* picture,SDL_Rect r){
	static SDL_Surface* surface=NULL;
	surface=IMG_Load(picture);

	SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_RenderCopy(renderer,texture,NULL,&r);
//	delete surface;
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return;
}

void add_align_center(SDL_Renderer* renderer,const char* picture,int center_x,int center_y,int w=0,int h=0){
	int& x=center_x;
	int& y=center_y;
	SDL_Surface* surface=IMG_Load(picture);
	SDL_Rect r;
	r.x=x-w/2;r.y=y-h/2;r.w=w;r.h=h;
	
	SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
	SDL_RenderCopy(renderer,texture,NULL,&r);
//	delete surface;
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return;
}

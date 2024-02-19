bool inside_rect(int x,int y,SDL_Rect rect){
	if(rect.x<=x and x<=rect.x+rect.w and 
			rect.y<=y and y<=rect.y+rect.h )
		return 1;
	else return 0;
}
class button{
	public:
		button(SDL_Renderer* rd , const char* c,int x,int y,int h,int w){
			renderer=rd;
			pic=c;
			rect.x=x;
			rect.y=y;
			rect.h=h;
			rect.w=w;
		} 
		void show(){ 
			add_picture(renderer,pic,rect);
		}
		bool inside(int x,int y){
			return inside_rect(x,y,rect);
		}
	private:
		SDL_Rect rect;
		const char* pic;
		SDL_Renderer* renderer;
};

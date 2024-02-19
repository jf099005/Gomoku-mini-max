#include "SDL2/SDL.h" 
#include"choose_color.h"
#include<vector>
#include<iostream>
using namespace std;
//#include"test_1228.h"
class chess_board{
	public:
		chess_board(SDL_Renderer*rd,int w,bool md,pair<int,int> cn):
				board(15+2,vector<int>(15+2,0)),wid(w),player(1),Computer(15)
		{	
			play_mode=md;//1:電腦對戰，0:雙人對戰 
			//corner.first=w;
			//corner.second=w;
			corner=cn;
			for(int i=0;i<15+2;i++){
				board[0][i]=10;
				board[i][0]=10;
				board[15+1][i]=10;
				board[i][15+1]=10;
			}
			renderer=rd;
		}
		//void show();
		void update();
		void move_chess();
		void play_game();
		void fresh(bool);
	private:
		const int size=14*20;
		vector< vector<int> >  board;
		SDL_Renderer*renderer;
		vector< pair<int,int> > points;
		pair<int,int> corner;
		int wid;
		int player=1;
		chess Computer;
		bool play_mode;//0:PVP,1:PVC
		
		inline bool end_page();
		inline pair<int,int> get_best_next(int);
};

void chess_board::fresh(bool end=0){
	SDL_RenderClear(renderer);
	add_picture(renderer,"./img/background.jpg",0,0,800,600);

	add_picture(renderer,"./img/board.jpg",corner.first,corner.second,wid*16,wid*16);
    
    if(!end){
		if(play_mode==1)
		    add_picture(renderer,"./img/PVC_Sign.png",620,250,180,60);
		else
		    add_picture(renderer,"./img/PVP_Sign.png",620,250,180,60);
	}
	
	static int x,y;
	static int p;
	p=1;
	if(points.size()){
		auto  last=*(points.end()-1);
		int lx=last.first,ly=last.second;
		add_align_center(renderer,"./img/chess_background.png",corner.first+lx*wid+wid,corner.second+ly*wid+wid,wid+5,wid+5);
	}
	for(auto it:points){
		x=it.first,y=it.second;
		char* pic="./img/black.png";
		if(p==-1) pic="./img/white.png";
		add_align_center(renderer,pic,corner.first+(x+1)*wid,corner.second+(y+1)*wid,wid,wid);
		p=-p;
	}
}

void chess_board::play_game(){//1:black win ,-1: white win, 0:tie

	SDL_Event event;
    int x=0,y=0;
	int x1,y1;
	add_picture(renderer,"./img/background.jpg",0,0,800,600);
	
	int color=0;
	if(play_mode==1){
		color=choose_color(renderer);
		if(color==0)return;
	}
	for(int i=0;i<=15;i++)
		for(int j=0;j<=15;j++)
			board[i][j]=0;
	player=1;
	while (1) {
		if(Computer.winner() or Computer.get_step()>=size*size-1)break;
		SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
        	//cout<<"BREAK\n";
            return;
        }
        else if(event.type == SDL_MOUSEMOTION){
 
        	x1=(event.motion.x-corner.first+wid/2)/wid;
			y1=(event.motion.y-corner.second+wid/2)/wid;
		//	cout<<"origin:"<<event.motion.x<<","<<event.motion.y<<"\n";
        //	cout<<x1<<","<<y1<<"\n";
			if(x!=x1 or y!=y1){
				fresh();
				
				if(x1>=0 and y1>=0 and x1<=14 and y1<=14)
					add_align_center(renderer,"./img/black.bmp",corner.first+x1*wid,corner.second+y1*wid,wid/2,wid/2);
				
			}/**/
		}
		else if(event.type==SDL_MOUSEBUTTONDOWN){
			x1=(event.motion.x-corner.first+wid/2)/wid -1;
			y1=(event.motion.y-corner.second+wid/2)/wid -1;
			//cout<<"(x1,y1):"<<x1<<","<<y1<<"\n";
			if(0<=x1 and x1<=14 and 0<=y1 and y1<=14 and board[x1][y1]==0){
		//		cout<<"add chess at"<<x1<<","<<y1<<"\n";
				points.push_back(pair<int,int>(x1,y1));
				board[x1][y1]=player;
				swap(x1,y1);
				Computer.move_chess(player,x1+1,y1+1);
				player=-player;
			}
			fresh();
			//Computer.print_board();
		if(Computer.winner() or Computer.get_step()>=size*size-1)break;
		}
		SDL_RenderPresent(renderer);
		if(player!=color and play_mode==1){
			//int k=3;
			//if(Computer.get_step()>=5)k=4;
			//if(Computer.get_step()>=15)k=5;
			//cout<<"step:"<<Computer.get_step()<<"\n";
			pair<int,int> nx=get_best_next(player);
			//Computer.bfs(player,k);
			x1=nx.first,y1=nx.second;
			Computer.move_chess(player,x1,y1);
			if(x1==-1 and y1==-1)return;
			x1--;y1--;
			//cout<<"nx:"<<x1<<","<<y1<<"\n";
			swap(x1,y1);
			if(0<=x1 and x1<=14 and 0<=y1 and y1<=14){
		//		cout<<"add chess at"<<x1<<","<<y1<<"\n";
				points.push_back(pair<int,int>(x1,y1));
				board[x1][y1]=player;
				player=-player;
				fresh();
			
			//	Computer.print_board();
			}
			else{
				cout<<"ERROR:invalid point:"<<x1<<","<<y1<<"\n";
				break;
			}
		}
		SDL_RenderPresent(renderer);
    }
    fresh(); 
    bool cont=end_page();
    
    if(cont){
    	//cout<<"next!\n";
    	Computer.reset();
    	points.clear();
		int md=start_page(renderer); 
		if(md==-1)return;
		else play_mode=md;
    	play_game();
	};
    //cout<<"end\n";
	return;
	//return 0;
}

inline pair<int,int> chess_board::get_best_next(int p){
	int k=3;	
	/*if(points.size()==0)return pair<int,int>(8,8);

	if(Computer.get_step()>=5)k=4;
	if(Computer.get_step()>=20)k=5;
	return Computer.bfs(player,k);/**/
	///*
	bool test=0;
	int t0=SDL_GetTicks(),t=0;
	//int k=3;	
	pair<int,int> ret;
	if(test)cout<<"================================\n";
	while(1){
		t0=SDL_GetTicks();
		if(test)Computer.print_board();
		ret=Computer.bfs(p,k);
		t=SDL_GetTicks()-t0;
		if(t*points.size()>1000 or k>=5)break;
		k++;
	}
	if(test)cout<<"mini_max:("<<ret.first<<","<<ret.second<<")\n";
	//cout<<"k="<<k<<",t="<<t<<"\n";
	return ret;/**/
}

inline bool chess_board::end_page(){
	SDL_Event event;	
	//window:(36*(14+5))^2
	button yes=button(renderer,"./img/yes.png" ,620,350,45,180);
	button no=button(renderer,"./img/no.png",620,450,45,180);
	//add_picture(renderer,"./img/background.jpg",0,0,800,600);
    
    int winner=Computer.winner();
    fresh(1);
    if(winner==1) 
	    add_picture(renderer,"./img/Black_win.png",130,260,180*2,45*2);
	else
	    add_picture(renderer,"./img/White_win.png",130,260,180*2,45*2);
		
        
    add_picture(renderer,"./img/continue.png",620,250,180,45);
	yes.show();
	no.show();
		
	while(1){
		//SDL_RenderClear(renderer);
		SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
        //	cout<<"BREAK\n";
            return 0;
        }
		if(event.type==SDL_MOUSEBUTTONDOWN){
			//cout<<"click at "<<event.motion.x<<','<<event.motion.y<<"\n";
			if(yes.inside(event.motion.x,event.motion.y)){
			//	cout<<"Yes\n";
				return 1;
			}
			if(no.inside(event.motion.x,event.motion.y))return 0;
		}
		SDL_RenderPresent(renderer);
	}
	
}


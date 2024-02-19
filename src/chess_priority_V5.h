//sparse+ 優先選點 + 5、活4剪枝
//有問題，電腦比起自己連活四會優先擋對手活三 

#include<bits/stdc++.h>
#define for0(x) for(int i=0;i<x;i++)
#define for1(x) for(int i=1;i<=x;i++)
#define fori(i,x) for(int i=0;i<x;i++)
using namespace std;
typedef pair<int,int> pii;

class chess{
	
	public:
		inline void init(int n,int mk);
		inline int get_k(int x,int y,int dx,int dy);
		inline pair<int,int> bfs(int p,int k,int wid);
		inline void move_chess(int p,int x,int y);
		inline void set_board(vector< vector<int> > board_0);
		inline void print_board();
		
		inline int get_step(){
			int s=step;
			return s;
		}
		inline int get_size(){
			return size;
		} 
		chess(int n):
			board(n+2,vector<int>(n+2,0)),size(n)
		{
			for0(n+2){
				board[i][0]=10;
				board[0][i]=10;
				board[n+1][0]=10;
				board[0][n+1]=10;
			}
		};
		
		inline int winner();
		inline bool check(pii pt);
		void reset(){
			fori(i,size){
				fori(j,size){
					board[i+1][j+1]=0;
				}
			}
			p1_val=0;
			p2_val=0;
			step=0;
		}
		
	private:
		
		vector< vector<int> > board;
		int step=0;
		int size;
		//int mxk=3;
		//int board_val=0;
		int p1_val=0;//val of p=1
		int p2_val=0;//val of p=-1
		const int dir[4][2]={{1,0},{0,1},{1,1},{1,-1}};
		const int point[3][6]={
			{0,20,100,500,2500,100000},
			{0,0,20,100,500,100000},
			{0,0,0,0,0,100000}
		};		
		
		inline int calculate(int p);
		
		bool sparse(int x,int y,int wid);
		
		inline int cal_diff(int p,int pi,pii pos);
		
		inline int vic(int,int,int);
		int mini_max(int p,int k,int a,int b,int wid);
		
		int visit(int p,int k,int a,int b,int wid,int x,int y);
};

inline int chess::get_k(int x,int y,int dx,int dy){
	int k=1;
	while(board[x][y]==board[x+dx*k][y+dy*k] and k<5)k++;
	return k;
}

inline int chess::calculate(int p){//count the score of player p (p=1 or -1)
	int n=board.size()-2;
	int ret=0;
	for(int x=1;x<=n;x++){
		for(int y=1;y<=n;y++){
			for(int d=0;d<4;d++){
				int dx=dir[d][0],dy=dir[d][1];
				if(board[x][y]!=p or board[x-dx][y-dy]==p){
					continue;
				}
				int k=get_k(x,y,dx,dy);
				int obs=(board[x-dx][y-dy]!=0)+(board[x+k*dx][y+k*dy]!=0);
				ret+=point[obs][k];
				
			}
		}
	}
	return ret;
}

inline int chess::cal_diff(int p,int pi,pii pos){
	int x=pos.first,y=pos.second;
	int ret=0;
	fori(d,4){
		int dx=dir[d][0],dy=dir[d][1];
		if(board[x-dx][y-dy]==pi and pi==board[x+dx][y+dy]){
			if(p==pi){
				int L=get_k(x-dx,y-dy,-dx,-dy),R=get_k(x+dx,y+dy,dx,dy);
				int oL=(board[x-L*dx-dx][y-L*dy-dy]!=0),oR=(board[x+dx+R*dx][y+dy+R*dy]!=0);
				
				int len=min(5,L+R+1);
				ret=ret-point[oL][L]-point[oR][R]+point[oL+oR][len];
			}
		}
		else if( p==pi and board[x-dx][y-dy]==0 and board[x+dx][y+dy]==0)ret+=point[0][1];
		else fori(j,2){
			if(j)dx=-dx,dy=-dy;
			if(board[x+dx][y+dy]!=p)continue;
			int k=get_k(x+dx,y+dy,dx,dy);
			if(k>5)k=5;
			int obs0=(board[x+k*dx+dx][y+k*dy+dy]!=0);
			int obs=(pi!=p or board[x-dx][y-dy]!=0)+(board[x+k*dx+dx][y+k*dy+dy]!=0);
			ret-=point[obs0][k];
			if(p==pi){
				k=min(k+1,5);
				obs=(board[x-dx][y-dy]!=0)+(board[x+k*dx][y+k*dy]!=0);
			}
			ret+=point[obs][k];
		}
	}
	return ret;
};

bool chess::sparse(int x,int y,int wid=3){
	if(wid<1)wid=1;
	for(int i=x-wid;i<=x+wid;i++){
		for(int j=y-wid;j<=y+wid;j++){
			if(i<=0 or i>size or j<=0 or j>size)continue;
			if(board[i][j])return 0;
		}
	}
	return 1;
}


int chess::visit(int p,int k,int a,int b,int wid,int x,int y){
	
	int origin_v1=p1_val,origin_v2=p2_val;
	p1_val += cal_diff(1,p,pii(x,y));
	p2_val += cal_diff(-1,p,pii(x,y));
	
	int dt=1;
	
	board[x][y]=p;
	int v=mini_max(-p,k-1,a,b,wid-dt);
	board[x][y]=0;
			
	p1_val=origin_v1;
	p2_val=origin_v2;
			
	return v;
}

inline int chess::vic(int pi,int x,int y){//return 是否下於(x,y)後可做出活4 or 5 
	int ret=0;
	if(board[x][y]!=0){
		cout<<"ERR in Vic:"<<x<<","<<y<<"\n";
		return -1;
	}
	board[x][y]=pi;
	fori(d,4){
		int dx=dir[d][0],dy=dir[d][1];
		int L=get_k(x,y,dx,dy),R=get_k(x,y,-dx,-dy);
		if(L+R-1>=5)ret= 5;
		if(L+R-1<4)continue;
		if((board[x+L*dx][y+L*dy]==0) and (board[x-R*dx][y-R*dy]==0))
			ret= 4;
	}
	board[x][y]=0;
	return ret;
}

int chess::mini_max(int p,int k,int a=-1e9,int b=1e9,int wid=3){//p=1:max , p=-1:mini ,a=min,b=max
	
	if(p!=1 and p!=-1){
		cout<<"Error in mini_max: invalid player: "<<p<<endl;
		return -1;
	}
	
	if(k==0)return p1_val-p2_val;//(calculate(1)-calculate(-1));
	
	if(k<0){
		cout<<"Error in mini_max: invalid k: "<<k<<endl;
		return -1;
	}
	
	if(p1_val>=1e5)return point[0][5];
	if(p2_val>=1e5)return -point[0][5];
	
	int n=board.size()-2;
	int mn_value=1e9,mx_value=-1e9;	
	
	bool priority_point[n+2][n+2];
	memset(priority_point,0,sizeof(priority_point));
	
	//set<pii> priority_point;
	int cut=1000;//1000:none, 0:p5,1:-p5, 2:pL4,3:-pL4
	int cut_x=1,cut_y=1;
	//直接結束剪枝 
	for(int x=1;x<=n;x++){
		for(int y=1;y<=n;y++){
			if(board[x][y]!=0){
				continue;
			};
			
			if(sparse(x,y,wid)){
				continue;
			}
			
			//int val_p=cal_diff(p,p,pii(x,y)),val_op=cal_diff(-p,-p,pii(x,y));
			int len_p=vic(p,x,y),len_op=vic(-p,x,y);
			
			if(len_p==5)return (p==1?point[0][5]-2:-point[0][5]+2);	
			if(len_op==5)cut=1,cut_x=x,cut_y=y;
			else if(cut>=2 and len_p==4)cut=2;
				
		}
	}
	if(cut!=1000){
		if(cut==2)return (p==1?point[2][5]+1:-point[2][5]+1);
		else if(cut==1)return visit(p,k,a,b,wid,cut_x,cut_y);
	}/**/
	
	//優先選點
	for(int x=1;x<=n;x++){
		for(int y=1;y<=n;y++){
			if(board[x][y]!=0){
				continue;
			};
			if(sparse(x,y,wid)){
				continue;
			}
			
			if(cal_diff(p,p,pii(x,y))>=500 or cal_diff(-p,-p,pii(x,y))>=500){
				
				int v=visit(p,k,a,b,wid,x,y);
				if(p==1 and v>mx_value ){
					mx_value=v;
					a=mx_value;
				}
				else if(p==-1 and v<mn_value){
					mn_value=v;
					b=mn_value;
				}
				
				mx_value=max(mx_value,v);
				mn_value=min(mn_value,v);			
				
				if(a>b)return (p==1?mx_value:mn_value);
				priority_point[x][y]=1;
				
			}
		}
	}

	//一般展開		
	for(int x=1;x<=n;x++){
		for(int y=1;y<=n;y++){
		//	int x=(x0+n-1)%n+1,y=(y0+n-1)%n+1;
			if(board[x][y]!=0)continue;
			if(sparse(x,y,wid))continue;
			if(priority_point[x][y])continue;
			//visit the point x,y
			int v=visit(p,k,a,b,wid,x,y);
			if(p==1 and v>mx_value ){
				mx_value=v;
				a=mx_value;
				if(v>=point[2][5])return v;
			}
			else if(p==-1 and v<mn_value){
				mn_value=v;
				b=mn_value;
				if(v<=-point[2][5])return v;
			}
					
			mx_value=max(mx_value,v);
			mn_value=min(mn_value,v);
			if(a>b)return (p==1?mx_value:mn_value);
		}
	}
	return (p==1?mx_value:mn_value);
}

inline pair<int,int> chess::bfs(int p,int k=2,int wid=4){ //p=1:max,p=-1:min
	if(step==0)return pair<int,int> (size/2,size/2);
	if(step==1)wid=2;
	int a=-1e9,b=1e9;
	int n=board.size()-2;
	pii ret=pii(-1,-1);
	vector<pii> ret_all;
	int mx_value=-1e9,mn_value=1e9;
	
	bool test=0;
	
	SDL_Event event;

	//直接結束剪枝
	int cut=1000;
	for(int x=1;x<=n;x++){
		for(int y=1;y<=n;y++){
			if(board[x][y]!=0){
				continue;
			};
			
			if(sparse(x,y,wid)){
		//		if(k==2)cout<<"("<<x<<","<<y<<") is sparse\n";
				continue;
			}
			
			int len_p=vic(p,x,y),len_op=vic(-p,x,y);
			
			if(len_p>=5)return pair<int,int>(x,y);
				
			if(len_op>=5){
				cut=1;
				ret=pair<int,int> (x,y);
			}
			else if(cut>=2 and len_p>=4){
				cut=2;
				ret=pair<int,int> (x,y);
			}
			//	else if(val_op>=4 and cp2>=point[0][4])cut=4;
				//priority_point[x][y]=1;
				
		}
	}
	if(cut!=1000){
		cout<<"priority_ret\n";
		return ret;
	}/**/
	



	//優先選點=============================================

	int priority_point[n+2][n+2];
	memset(priority_point,0,sizeof(priority_point));
	
	for(int x=1;x<=n;x++){
		for(int y=1;y<=n;y++){
			
			SDL_PollEvent(&event);
	        if (event.type == SDL_QUIT) {
	        	//cout<<"BREAK\n";
	            return pii(-1,-1);
	        }
			
			if(board[x][y]!=0){
				continue;
			};
			
			if(sparse(x,y,wid)){
				continue;
			}
			int val_p=cal_diff(p,p,pii(x,y)),val_op=cal_diff(-p,-p,pii(x,y));			
			if(max(val_p,val_op)>=point[0][3]){
				priority_point[x][y]=1;
				int v=visit(p,k,a,b,wid,x,y);
				
				if(p==1 and  v>mx_value ){
					//ret=pii(x,y);
					mx_value=v;
					a=mx_value;
					ret_all.clear();
					ret_all.push_back(pii(x,y));
				}
				
				if(p==-1 and v<mn_value){
					//ret=pii(x,y);
					mn_value=v;
					b=mn_value;
					ret_all.clear();
					ret_all.push_back(pii(x,y));
				}
	
				mx_value=max(mx_value,v);
				mn_value=min(mn_value,v);
				priority_point[x][y]=v;
			}
		} 
	}/**/
	//===================================================== 
		
	for(int x=1;x<=n;x++){for(int y=1;y<=n;y++){
		
			if(board[x][y]!=0){
				if(test)cout<<setw(6)<<(board[x][y]==1?'A':'B')<<' ';
				continue;
			};
			
			if(sparse(x,y,wid)){
				if(test)cout<<setw(6)<<" [] "<<' ';
				continue;
			}
			
			if(priority_point[x][y]){
				if(test)cout<<setw(6)<<priority_point[x][y]<<'*';
				continue;
			}
			
			int v=visit(p,k,a,b,wid,x,y);
			
			if(test)cout<<setw(6)<<(v>=1e6?1e6-1:(v<=-1e6?-1e6+1:v))<<" ";
			
			if(p==1 and  v>mx_value ){
				mx_value=v;
				a=mx_value;
				ret_all.clear();
				ret_all.push_back(pii(x,y));
			}
			else if(p==1 and v==mx_value){
				ret_all.push_back(pii(x,y));
			}
			
			if(p==-1 and v<mn_value){
				mn_value=v;
				b=mn_value;
				ret_all.clear();
				ret_all.push_back(pii(x,y));
			}
			else if(p==-1 and v==mn_value){
				ret_all.push_back(pii(x,y));
			}
			
			board[x][y]=0;
		}
		if(test)cout<<"\n";
	}
	
	if( (p==1 and mx_value==-point[2][5]) or (p==-1 and mn_value==point[2][5]) ){
		int x=ret_all[0].first,y=ret_all[0].second;
		
		int origin_v1=p1_val,origin_v2=p2_val;
		p1_val+=cal_diff(1,p,pii(x,y));
		p2_val+=cal_diff(-1,p,pii(x,y));
		
		board[x][y]=p;
		ret=bfs(-p,k-1,wid-1);
		board[x][y]=0;
		
		p1_val=origin_v1;
		p2_val=origin_v2;
		
		return ret;
	}
	
	return ret_all[rand()%ret_all.size()];
}

inline void chess::move_chess(int pi,int x,int y){
	if(x<=0 or x>size or y<=0 or y>size or board[x][y]!=0){
		cout<<"invalid point:("<<x<<","<<y<<"):";
		if(x<=0 or x>size or y<=0 or y>size)cout<<" exceed bound\n";
		else cout<<"the point is not empty\n";
	}
	step++;
	p1_val+=cal_diff(1,pi,pii(x,y));
	p2_val+=cal_diff(-1,pi,pii(x,y));
	board[x][y]=pi;
}

inline void chess::set_board(vector< vector<int> > board_0){
	board=board_0;
	p1_val=calculate(1);
	p2_val=calculate(-1);
}

inline void chess::print_board(){
	for0(board.size()-1)cout<<setw(3)<<i;
	cout<<endl;
	for(int i=1;i<board.size()-1;i++){
		cout<<setw(3)<<i;
		for(int j=1;j<board.size()-1;j++){
			cout<<setw(3)<<(board[i][j]==0?'_':(board[i][j]==1?'O':'X'));
		}
		cout<<endl;
	}
}

inline bool chess::check(pii pt){
	return (board[pt.first][pt.second]?0:1);
}

inline int chess::winner(){// if the game is still not end, return 0
	if(p1_val>=point[2][5])return 1;
	if(p2_val>=point[2][5])return -1;
	else return 0;
}

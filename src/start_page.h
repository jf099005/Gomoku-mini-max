int start_page(SDL_Renderer* renderer){
	SDL_Event event;	
	button pc=button(renderer,"./img/PVC.png" ,620,250,45,180);
	button p2=button(renderer,"./img/PVP.png" ,620,360,45,180);
	while(1){
		SDL_RenderClear(renderer);
		SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            return -1;
        } 
        add_picture(renderer,"./img/background.jpg",0,0,800,600);
		pc.show();
		p2.show();
		if(event.type==SDL_MOUSEBUTTONDOWN){
			if(pc.inside(event.motion.x,event.motion.y))return 1;
			if(p2.inside(event.motion.x,event.motion.y))return 0;
		}
		SDL_RenderPresent(renderer);
	}
	
}

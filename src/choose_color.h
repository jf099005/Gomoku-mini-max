int choose_color(SDL_Renderer* renderer){
	SDL_Event event;	
	//window:(36*(14+5))^2
	button black=button(renderer,"./img/black.png" ,620,300,45,45);
	button white=button(renderer,"./img/white.png" ,620,360,45,45);
	
	add_picture(renderer,"./img/choose_color.png",620,250,180,45);

	black.show();
	white.show();	
	SDL_RenderPresent(renderer);

	while(1){
		//SDL_RenderClear(renderer);
		SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
        	cout<<"BREAK\n";
            return 0;
        }
        //add_picture(renderer,"./img/background.jpg",0,0,800,600);

		if(event.type==SDL_MOUSEBUTTONDOWN){
			//cout<<"click at "<<event.motion.x<<','<<event.motion.y<<"\n";
			if(black.inside(event.motion.x,event.motion.y))return 1;
			if(white.inside(event.motion.x,event.motion.y))return -1;
		}
	}
	
}

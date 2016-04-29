#include "View.h"

using namespace std;

View::View(int width, int height, bool full_screen){
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	
	if (full_screen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	Graphic::makeTemplateOfCircle(Graphic::radius_of_circle_template);
}

View::~View() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void View::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void View::draw(Object * obj) {
	obj->move(position);
	obj->draw(renderer);
	obj->move(!position);
}

void View::swapBuffers() {
	SDL_RenderPresent(renderer);
}




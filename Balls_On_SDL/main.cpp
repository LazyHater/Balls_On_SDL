#include "main.h"
#include "Environment.h" 
using namespace concurrency;
using namespace std;

//int WINDOW_WIDTH = 1920;
//int WINDOW_HEIGHT = 1080;

//int WINDOW_WIDTH = 1366;
//int WINDOW_HEIGHT = 768;

int WINDOW_WIDTH = 1000;
int WINDOW_HEIGHT = 700;

//int WINDOW_WIDTH = 640;
//int WINDOW_HEIGHT = 480;

bool full_screen = false;

float FPS_MAX = 60.0f;
float frame_time, FPS;

Environment environment(Rectangle(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT)));


int main(int argc, char *argv[]) {
	handleCommandLine(argc, argv);
	View view(WINDOW_WIDTH, WINDOW_HEIGHT, full_screen);
	
	//environment.rectangles.push_back(Rectangle(Vector2D(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 200, 200));
//	environment.rectangles[0].color = RGB(0, 255, 0);

	float start;
	environment.precision_of_calcs = 1;

	environment.BSpwn.deployBalls(Vector2D(200, 100), 1, false);
	environment.BSpwn.deployBalls(Vector2D(400, 200), 1, false);
	environment.BSpwn.deployBalls(Vector2D(150, 300), 1, false);
	environment.BSpwn.balls[1].m = 5000;

	environment.BSpwn.balls[0].velocity = Vector2D(100, 0);
	environment.BSpwn.balls[2].velocity = Vector2D(100, 20);

	environment.gravity_vector = Vector2D(0, 0);

	while (1) {
		task<void> drawTask([&view]() {
			view.clear();

			for (Ball ball : environment.BSpwn.balls)
				view.draw(&ball);

			for (Rectangle rectangle : environment.rectangles)
				view.draw(&rectangle);

			for (Line line : environment.lines)
				view.draw(&line);
			view.draw(&environment.box);
			view.swapBuffers();
		});
		
		environment.update(frame_time);

		bool quit = false;
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			int x, y;
			static Vector2D  vl;
			static bool line_flag = false;
			static float scale = 1.0f; // works like shiet, but works... not at all, but still...
			switch (event.type)	{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_LEFT))) {
						drawTask.wait();
						environment.BSpwn.deployBalls(Vector2D(x, y) - view.position, 1, true, false);
					}

					if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_RIGHT))) {
						drawTask.wait();
						environment.BSpwn.balls.clear();
					}

					if ((SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE))) && (!line_flag)) {
						environment.lines.push_back(Line(Vector2D(x, y), Vector2D(x + 1, y + 1),
							RGB(0, randFromTo(50, 255), randFromTo(50, 255))));
						environment.lines[environment.lines.size() - 1].move(!view.position);
						line_flag = true;
					}
					break;
				case SDL_MOUSEMOTION:
					if (line_flag) {
						environment.lines[environment.lines.size() - 1].p2.set(event.motion.x, event.motion.y);
						environment.lines[environment.lines.size() - 1].p2 -= view.position;
						environment.lines[environment.lines.size() - 1].update();
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (!(SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE))))
						line_flag = false;
					break;
				case SDL_MOUSEWHEEL:
					scale += 0.1f*event.wheel.y;
					SDL_RenderSetScale(view.renderer, scale, scale);
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_r:
						view.position.set(0, 0);
						break;
					case SDLK_d:
						environment.lines.clear();
						break;
					case SDLK_f:
						environment.BSpwn.ball_to_ball_bounce = !environment.BSpwn.ball_to_ball_bounce;
						break;
					case SDLK_g:
						if (environment.gravity_vector == Vector2D(0, 0))
							environment.gravity_vector = Vector2D(0, 100);
						else
							environment.gravity_vector = Vector2D(0, 0);
						break;
					case SDLK_KP_PLUS:
						environment.speed_of_simulation += 0.1f;
						break;
					case SDLK_KP_MINUS:
						environment.speed_of_simulation -= 0.1f;
						break;
					case SDLK_RIGHT:
						view.position.x -= 10;
						break;
					case SDLK_LEFT:
						view.position.x += 10;
						break;
					case SDLK_UP:
						view.position.y += 10;
						break;
					case SDLK_DOWN:
						view.position.y -= 10;
						break;
					case SDLK_ESCAPE:
						quit = true;
						break; 
					} //end of switch (event.key.keysym.sym)
					break; 
			} // end of switch(event.type)
		}

		drawTask.wait();
		if (quit) break;

		frame_time = calcFrameTime(FPS_MAX);
		FPS = 1.0f / frame_time;

		//showInfo(100);
	}
	return EXIT_SUCCESS;
}

float calcFrameTime(float FPS_MAX) {
	static float start = clock();
	float frame_time = deltaTime(start);
	start = clock();
	float time_left = 1.0f/FPS_MAX - frame_time;
	if (time_left > 0) {
		SDL_Delay(time_left*1000.0f);
		frame_time += deltaTime(start);
	}
	return frame_time;
}

float deltaTime(float start) { //calc time from start in sec
	return ((clock() - start)/ CLOCKS_PER_SEC);
}

void handleCommandLine(int argc, char *argv[]) {
	for (int i = 1; i < argc; i++)	{
		if (commandCheck("-g")) environment.gravity_vector = Vector2D(0, 100);
		if (commandCheck("-e")) environment.BSpwn.ball_to_ball_collision = true;
		if (commandCheck("-bf")) environment.BSpwn.ball_to_ball_bounce = true;
		if (commandCheck("-f")) full_screen = true;
		if (commandCheck("-w"))  WINDOW_WIDTH = atoi(argv[i+1]);
		if (commandCheck("-h"))  WINDOW_HEIGHT = atoi(argv[i+1]);
		if (commandCheck("-p"))  environment.precision_of_calcs = atoi(argv[i + 1]);
		//if (commandCheck("-rrb"))  environment.BSpwn.random_radius = true; need to think about that
	}
}

void showInfo(int delay) {
	static int start = delay;
	if (deltaTime(start)*1000.0f >= delay) {
		start = clock();
		P(Ball::n);
		P(FPS);
		P(environment.speed_of_simulation);
		cout << endl;
	}
}


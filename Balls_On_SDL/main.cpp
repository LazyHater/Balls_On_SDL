#include "main.h"
using namespace concurrency;
using namespace std;

//int WINDOW_WIDTH = 1920;
//int WINDOW_HEIGHT = 1080;

//int WINDOW_WIDTH = 1366;
//int WINDOW_HEIGHT = 768;

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;

bool full_screen = false;

int radius_of_balls = 5;

float FPS_MAX = 60.0f;
float frame_time, FPS;
int precision_of_calcs = 10;  //the more the better
int balls_per_deploy = 50;

BallSpawner BSpwn;
vector<Line> lines;

void showInfo(int delay);

int main(int argc, char *argv[]) {
	handleCommandLine(argc, argv);
	View view(WINDOW_WIDTH, WINDOW_HEIGHT, full_screen);
	view.makeTemplateOfCircle(radius_of_balls);

	float start;
	while (1) {
		task<void> drawTask([&view]() {view.draw(BSpwn,lines);});
		
		for (int i = 0; i < precision_of_calcs; i++) {
			handleCollisionWithScreen(BSpwn.balls);
			handleCollisionWithLines(BSpwn.balls, lines);
			BSpwn.update(frame_time / precision_of_calcs);
		}

		bool quit = false;
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			int x, y;
			Vector2D static vl;
			bool static line_flag = false;

			if (event.type == SDL_QUIT) quit = true;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_LEFT)))
					BSpwn.deployBalls(Vector2D(x, y), balls_per_deploy);

				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_RIGHT)))
					BSpwn.balls.clear();	

				if ((SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE)))&&(!line_flag)) {
					lines.push_back(Line(Vector2D(x, y), Vector2D(x + 1, y + 1),
									RGB(0, randFromTo(50, 255), randFromTo(50, 255))));
					line_flag = true;
				}
			}

			if ((event.type == SDL_MOUSEMOTION)&&(line_flag)) {
					lines[lines.size() - 1].p2.set(event.motion.x, event.motion.y);
					lines[lines.size() - 1].update();
				}
				
			if (event.type == SDL_MOUSEBUTTONUP) {
				if (!(SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE))))
					line_flag = false;			
			}
			

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_g) BSpwn.gravity = !BSpwn.gravity;
				if (event.key.keysym.sym == SDLK_d) lines.clear();
				if (event.key.keysym.sym == SDLK_f) BSpwn.ball_to_ball_bounce = !BSpwn.ball_to_ball_bounce;
			}
		}
		if (quit) break;

		drawTask.wait();		
		
		frame_time = calcFrameTime(FPS_MAX);
		FPS = 1.0f / frame_time;
		
		showInfo(500);
	}
	return EXIT_SUCCESS;
}

void handleCollisionWithScreen(vector<Ball> &balls) {
	for (Ball &ball : balls) {
		if (ball.position.x + ball.r>WINDOW_WIDTH) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = WINDOW_WIDTH - ball.r;
			ball.collided = true;
		}
		if (ball.position.x - ball.r < 0)	{
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = ball.r;
			ball.collided = true;
		}
		if (ball.position.y + ball.r > WINDOW_HEIGHT)	{
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = WINDOW_HEIGHT - ball.r;
			ball.collided = true;
		}
		if (ball.position.y - ball.r < 0)	{
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = ball.r;
			ball.collided = true;
		}
	}

}

void handleCollisionWithLines(vector<Ball> &balls, vector<Line> lines) {
	for (Line line : lines) {
		for (Ball &ball : balls) {
			if (line.collideBox.isIn(ball.position)) {
				float distance = line.distance(ball.position) - line.width;
				if (distance < ball.r) {
					ball.velocity = ball.velocity - line.norVec*
						2.0*(ball.velocity*line.norVec); //R=V-2N(N*V)
					ball.collided = true;
					if (ball.velocity.angle(line.norVec) < M_PI / 2)
						ball.position += line.norVec*(ball.r - distance);
					else
						ball.position -= line.norVec*(ball.r - distance);
				}
			}
		}
	}
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
		if (commandCheck("-g")) BSpwn.gravity = true;
		if (commandCheck("-e")) BSpwn.ball_to_ball_collision = true;
		if (commandCheck("-bf")) BSpwn.ball_to_ball_bounce = true;
		if (commandCheck("-f")) full_screen = true;
		if (commandCheck("-w"))  WINDOW_WIDTH = atoi(argv[i+1]);
		if (commandCheck("-h"))  WINDOW_HEIGHT = atoi(argv[i+1]);
	}
}

void showInfo(int delay) {
	static int start = delay;
	if (deltaTime(start)*1000.0f >= delay) {
		start = clock();
		P(Ball::n);
		P(FPS);
		cout << endl;
	}
}
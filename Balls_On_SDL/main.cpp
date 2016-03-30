#include "main.h"
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

int radius_of_balls = 5;

float FPS_MAX = 60.0f;
float frame_time, FPS;
int precision_of_calcs = 1;  //the more the better
int balls_per_deploy = 100;

BallSpawner BSpwn;
vector<Line> lines;
vector<Rectangle> rectangles;



int main(int argc, char *argv[]) {
	handleCommandLine(argc, argv);
	View view(WINDOW_WIDTH, WINDOW_HEIGHT, full_screen);
	
	rectangles.push_back(Rectangle());
	rectangles[0].set(Vector2D((WINDOW_WIDTH / 2) - 100, (WINDOW_HEIGHT / 2) - 100), Vector2D((WINDOW_WIDTH / 2) + 100, (WINDOW_HEIGHT / 2) + 100));
	rectangles[0].set(Vector2D(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), 200, 200);
	rectangles[0].color = RGB(255, 0, 0);

	float start;
	while (1) {
		task<void> drawTask([&view]() {
			view.clear();

			for (Ball ball : BSpwn.balls)
				view.draw(&ball);

			for (Rectangle rectangle : rectangles)
				view.draw(&rectangle);

			for (Line line : lines)
				view.draw(&line);
			view.swapBuffers();
		});
		
		for (int i = 0; i < precision_of_calcs; i++) {
			handleCollisionWithScreen(BSpwn.balls);
			handleCollisionWithLines(BSpwn.balls, lines);
			handleCollisionWithRectangles(BSpwn.balls, rectangles);
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
				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_LEFT))) {
					drawTask.wait();
					BSpwn.deployBalls(Vector2D(x, y), balls_per_deploy);
				}

				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_RIGHT)))		{
					drawTask.wait();
					BSpwn.balls.clear();
				}

				if ((SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE))) && (!line_flag)) {
					lines.push_back(Line(Vector2D(x, y), Vector2D(x + 1, y + 1),
						RGB(0, randFromTo(50, 255), randFromTo(50, 255))));
					line_flag = true;
				}
			}

			if (event.type == SDL_MOUSEMOTION) {
				if (line_flag) {
					lines[lines.size() - 1].p2.set(event.motion.x, event.motion.y);
					lines[lines.size() - 1].update();
				}
				//Vector2D temp(event.motion.x, event.motion.y);
				//temp -= Vector2D(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
				//BSpwn.gravity_vector = temp;
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

		drawTask.wait();
		if (quit) break;

		frame_time = calcFrameTime(FPS_MAX);
		FPS = 1.0f / frame_time;

		showInfo(100);
	}
	return EXIT_SUCCESS;
}

void handleCollisionWithScreen(vector<Ball> &balls) {
	for (Ball &ball : balls) {
		if (ball.position.x + ball.r > WINDOW_WIDTH) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = WINDOW_WIDTH - ball.r;
			ball.collided = true;
		}
		if (ball.position.x - ball.r < 0) {
			ball.velocity.x = -ball.velocity.x;
			ball.position.x = ball.r;
			ball.collided = true;
		}
		if (ball.position.y + ball.r > WINDOW_HEIGHT) {
			ball.velocity.y = -ball.velocity.y;
			ball.position.y = WINDOW_HEIGHT - ball.r;
			ball.collided = true;
		}
		if (ball.position.y - ball.r < 0) {
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

void handleCollisionWithRectangles(vector<Ball> &balls, vector<Rectangle> &rectangles) {
	for (Rectangle &rectangle : rectangles) {
		for (Ball &ball : balls) {
			if ((abs(rectangle.position.x - ball.position.x)<ball.r + rectangle.width / 2.0f)
				&&(abs(rectangle.position.y - ball.position.y)<ball.r + rectangle.height / 2.0f)) {		

				if (abs(ball.position.x - rectangle.position.x + rectangle.width / 2.0f) < ball.r)	{
					ball.velocity.x = -ball.velocity.x*ball.bounce_factor;
					ball.position.x = rectangle.position.x - rectangle.width / 2.0f - ball.r;
				}

				if (abs(ball.position.x - rectangle.position.x - rectangle.width / 2.0f) < ball.r) {
					ball.velocity.x = -ball.velocity.x*ball.bounce_factor;;
					ball.position.x = rectangle.position.x + rectangle.width / 2.0f + ball.r;
				}

				if (abs(ball.position.y - rectangle.position.y - rectangle.height / 2.0f) < ball.r) {
					ball.velocity.y = -ball.velocity.y*ball.bounce_factor;;
					ball.position.y = rectangle.position.y + rectangle.height / 2.0f + ball.r;
				}

				if (abs(ball.position.y - rectangle.position.y + rectangle.height / 2.0f) < ball.r) {
					ball.velocity.y = -ball.velocity.y*ball.bounce_factor;;
					ball.position.y = rectangle.position.y - rectangle.height / 2.0f - ball.r;
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
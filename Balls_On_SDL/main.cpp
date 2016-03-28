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

float FPS_MAX = 30.0;
int balls_per_deploy = 100;

BallSpawner BSpwn;
vector<Line> lines;

int main(int argc, char *argv[]) 
{
	handleCommandLine(argc, argv);
	srand(time(NULL));
	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *window;
	makeTemplateOfCircle(BSpwn.radius_of_balls);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

	if (full_screen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	float start;
	while (1) {

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		
		task<void> drawTask([&renderer]()
		{
			draw(renderer);
		});

		//P(Ball::n);
		//measureTime(
			handleCollisionWithScreen(BSpwn.balls);		
			handleCollisionWithLines(BSpwn.balls, lines);
				//		, colis);

				//measureTime(
				BSpwn.update();
		//	,bspwn);
			
		drawTask.wait();
		
		float fps = doFPSStuff(FPS_MAX);
		//P(fps) << endl;

		SDL_RenderPresent(renderer);

		bool quit = false;
		if (SDL_PollEvent(&event))
		{
			Vector2D static vl;
			bool static line_flag = false;

			if (event.type == SDL_QUIT) quit = true;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_LEFT)))
				{
					BSpwn.deployBalls(Vector2D(x, y), balls_per_deploy);
				}

				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_RIGHT)))
				{
					BSpwn.balls.clear();
				}

				if (SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE)))
				{
					if (!line_flag)
					{
						vl.set(x, y);
						lines.push_back(Line(vl, Vector2D(x + 1, y + 1),
							RGB(0, randFromTo(50, 255), randFromTo(50, 255))));
						line_flag = true;
					}
				}
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				if (line_flag)
				{
					vl.set(event.motion.x, event.motion.y);
					lines[lines.size() - 1].p2 = vl;
					lines[lines.size() - 1].update();
				}
				

			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				if (!(SDL_GetMouseState(&x, &y)&(SDL_BUTTON(SDL_BUTTON_MIDDLE))))
					line_flag = false;			
			}
			

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_d) lines.clear();
				if (event.key.keysym.sym == SDLK_f) BSpwn.ball_to_ball_bounce_factor = !BSpwn.ball_to_ball_bounce_factor;
			}
		}
		if (quit) break;
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

void handleCollisionWithScreen(vector<Ball> &balls)
{

	for (int i = 0; i < balls.size(); i++)
	{
		if (balls[i].position.x + balls[i].velocity.x + balls[i].r>WINDOW_WIDTH)
		{
			balls[i].velocity.x = -balls[i].velocity.x;
			balls[i].position.x = WINDOW_WIDTH - balls[i].r;
			balls[i].collided = true;
		}
		if (balls[i].position.x + balls[i].velocity.x - balls[i].r < 0)
		{
			balls[i].velocity.x = -balls[i].velocity.x;
			balls[i].position.x = balls[i].r;
			balls[i].collided = true;
		}
		if (balls[i].position.y + balls[i].velocity.y + balls[i].r > WINDOW_HEIGHT)
		{
			balls[i].velocity.y = -balls[i].velocity.y;
			balls[i].position.y = WINDOW_HEIGHT - balls[i].r;
			balls[i].collided = true;
		}
		if (balls[i].position.y + balls[i].velocity.y - balls[i].r < 0)
		{
			balls[i].velocity.y = -balls[i].velocity.y;
			balls[i].position.y = balls[i].r;
			balls[i].collided = true;
		}
	}

}

void handleCollisionWithLines(vector<Ball> &balls, vector<Line> &lines)
{
	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = 0; j < balls.size(); j++)
		{
			if (lines[i].collideBox.isIn(balls[j].position))
			{
				float distance = lines[i].distance(balls[j].position)-2;
				if (distance < balls[j].r)
				{
					balls[j].velocity = balls[j].velocity - lines[i].norVec*
						2.0*(balls[j].velocity*lines[i].norVec); //R=V-2N(N*V)
					balls[j].collided = true;
					if (balls[j].velocity.angle(lines[i].norVec) < M_PI / 2)
						balls[j].position += lines[i].norVec*(balls[j].r - distance);
					else
						balls[j].position -= lines[i].norVec*(balls[j].r - distance);
				}
			}
		}
	}
}

float doFPSStuff(float FPS)
{
	static float start = clock();

	float frame_time = deltaTime(start);
	start = clock();
	float time_left = 1000.0f/FPS - frame_time;
	if (time_left > 0)
	{
		SDL_Delay(time_left);
		frame_time += time_left;
	}
	
	return 1000.0f/frame_time;
}

float deltaTime(float start)
{
	return ((clock() - start)*1000.0f / CLOCKS_PER_SEC);
}

void draw(SDL_Renderer *renderer)
{
	for (Line line : lines)
		renderLine(renderer, line.p1, line.p2, line.color);

	for (Ball ball : BSpwn.balls)
		renderCircle(renderer, ball.position, ball.r, ball.color);
	
}

void handleCommandLine(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (commandCheck("-g")) BSpwn.gravity = true;
		if (commandCheck("-e")) BSpwn.ball_to_ball_collision = true;
		if (commandCheck("-bf")) BSpwn.ball_to_ball_bounce_factor = true;
		if (commandCheck("-f")) full_screen = true;
		if (commandCheck("-w"))  WINDOW_WIDTH = atoi(argv[i+1]);
		if (commandCheck("-h"))  WINDOW_HEIGHT = atoi(argv[i+1]);
	}
}

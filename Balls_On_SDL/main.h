#include <cmath>
#include <ctime>
#include <vector>
#include <ppltasks.h>
#include "SDL.h"
#include "graphics.h"
#include "BallSpawner.h"
#include "Line.h"

void handleCollisionWithScreen(std::vector<Ball> &balls);
void handleCollisionWithLines(std::vector<Ball> &balls, std::vector<Line> &lines);
void handleCommandLine(int argc, char *argv[]);

float doFPSStuff(float FPS); // holds fps on FPS level
float deltaTime(float start);  // calculate delta time
void draw(SDL_Renderer *renderer); // main draw

#define P(x) cout<<#x<<": "<<x<<" "
#define	commandCheck(x) !strcmp(x,argv[i])
#define measureTime(x,y) float y=clock(); x; y=deltaTime(y); P(y)
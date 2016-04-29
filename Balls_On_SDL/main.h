#include <ctime>
#include <vector>
#include <ppltasks.h>
#include "SDL.h"
#include "graphics.h"
#include "BallSpawner.h"
#include "Line.h"
#include "View.h"

void handleCommandLine(int argc, char *argv[]);

float calcFrameTime(float FPS); // holds fps on FPS level
float deltaTime(float start);  // calculate delta time
void showInfo(int delay);


#define P(x) cout<<#x<<": "<<x<<" "
#define	commandCheck(x) !strcmp(x,argv[i])
#define measureTime(x,y) float y=clock(); x; y=deltaTime(y); P(y)
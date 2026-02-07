#include <cstdint>
#include "commonvars.h"
#include "Framebuffer.h"
#include "cboardparts.h"
#include "cselector.h"
#include "cmainmenu.h"

Framebuffer fb;

//game
CSelector *GameSelector;
bool PrintFormShown = false;
CBoardParts* BoardParts; // boardparts instance that will hold all the boardparts
int Difficulty = VeryEasy;
int Moves = 0;
int BestPegsLeft[4]; // array that holds the best amount of pegs left for each difficulty

//titlescreen
CMainMenu* Menu;

//main
uint8_t prevButtons, currButtons;
int showFps = false;
unsigned int prevLogTime;
unsigned int FrameTime, Frames;
float CurrentMs;
bool debugMode = false;
int GameState = GSTitleScreenInit; // the game state


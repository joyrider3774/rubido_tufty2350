#ifndef COMMONVARS_H
#define COMMONVARS_H

#include <cstdint>
#include "framebuffer.h"
#include "cselector.h"
#include "cmainmenu.h"

#define PROGMEM

typedef struct CBoardParts CBoardParts;
typedef struct CPeg CPeg;
typedef struct SPoint SPoint;
typedef struct CSelector CSelector;



// The diffrent difficultys
#define VeryEasy 0
#define Easy 1
#define Hard 2
#define VeryHard 3

// The diffrent gameStates possible in the game
#define GSQuit 0
#define GSGame 1 
#define GSTitleScreen 2
#define GSDifficultySelect 3 
#define GSCredits 4

#define GSInitDiff 50

#define GSGameInit (GSGame + GSInitDiff)
#define GSTitleScreenInit (GSTitleScreen + GSInitDiff)
#define GSDifficultySelectInit (GSDifficultySelect + GSInitDiff)
#define GSCreditsInit (GSCredits + GSInitDiff)

// window size
#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240

//game defines
#define NrOfRows 9
#define NrOfCols 9
#define TileWidth 24
#define TileHeight 24
#define IDPeg 1
#define XOffSet 10
#define YOffSet 11

#define COLOR_SWAP(c) ((uint16_t)((((c) >> 8) | (c) << 8)))
#define COLOR(r,g,b) COLOR_SWAP(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

#define BUTTON_A_MASK (1<<0)
#define BUTTON_B_MASK (1<<1)
#define BUTTON_C_MASK (1<<2)
#define BUTTON_UP_MASK (1<<3)
#define BUTTON_DOWN_MASK (1<<4)
#define BUTTON_HOME_MASK (1<<5)

#define FPS 30

#define COLOR_BACKGROUND COLOR(100,120,255)
#define COLOR_FOREGROUND COLOR(0,10,255)
#define COLOR_TRANSPARENT COLOR(0,11,255)
extern Framebuffer fb;

//game
extern CSelector *GameSelector;
extern bool PrintFormShown;
extern int Moves;
extern int BestPegsLeft[4]; // array that holds the best amount of pegs left for each difficulty
extern int Difficulty;
extern CBoardParts* BoardParts; // boardparts instance that will hold all the boardparts

//titlescreen
extern CMainMenu* Menu;

//main
extern uint8_t prevButtons, currButtons;
extern int showFps;
extern unsigned int prevLogTime;
extern unsigned int FrameTime, Frames;
extern float CurrentMs;
extern bool debugMode;
extern int GameState; // the game state

#endif
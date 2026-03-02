#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <cstring>
#include <cstdio>
#include "sound.h"
#include "cboardparts.h"
#include "cmainmenu.h"
#include "cpeg.h"
#include "cselector.h"
#include "commonvars.h"
#include "framebuffer.h"
#include "savestate.h"
#include "glcdfont.h"
#include "images/veryeasy1_RGB565_BE.h"
#include "images/veryhard1_RGB565_BE.h"
#include "images/hard1_RGB565_BE.h"
#include "images/easy1_RGB565_BE.h"
#include "images/credits_RGB565_BE.h"
#include "images/infoeasy_RGB565_BE.h"
#include "images/infohard_RGB565_BE.h"
#include "images/infoveryeasy_RGB565_BE.h"
#include "images/infoveryhard_RGB565_BE.h"
#include "images/background_RGB565_BE.h"

void resetGlobals()
{
	PrintFormShown = false;
	Difficulty = VeryEasy;
	Moves = 0;
	GameState = GSTitleScreenInit;
}

void setupGame()
{
	resetGlobals();
	loadSavedState();
	BoardParts = CBoardParts_Create();
	Menu = CMainMenu_Create();
	GameSelector = CSelector_Create(4,4);
}

void terminateGame()
{
	CBoardParts_Destroy(BoardParts);
	CMainMenu_Destroy(Menu);
	CSelector_Destroy(GameSelector);
	saveSavedState();
}

// procedure that calculates how many moves are possible in the current board state
// we can simply do this by checking all parts and see if they can move to all directions
// the canmoveto method in CPegs is does all the checking
int MovesLeft()
{
    CPeg *BoardPart ;
    int TmpMoves=0,Y,X;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            BoardPart = CBoardParts_GetPart(BoardParts, X,Y);
            // if there is a boardpart on that X,Y Coordinate
            // check all direction if we can move to that if so increases the movesleft
            if (BoardPart)
            {
                if (CPeg_CanMoveTo(BoardPart,X+2,Y,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X,Y-2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X+2,Y-2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X+2,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y-2,false))
                    TmpMoves++;
            }
        }
    return TmpMoves;
}

// Procedure that counts how many pegs are left on the board (animphase must be lower then 2 io a red or blue peg)
int PegsLeft()
{
    CPeg *BoardPart ;
    int Pegs=0,Y,X;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            BoardPart = CBoardParts_GetPart(BoardParts,X,Y);
            if (BoardPart)
                if ((CPeg_GetType(BoardPart) == IDPeg) && (CPeg_GetAnimPhase(BoardPart) < 2))
                    Pegs++;
        }
    return Pegs;
}

// procedure that draws the board, boardparts info and a boxed message over the playfield
// and waits till the A button is pressed

void PrintForm(const char *msg)
{
	PrintFormShown = true;
	bufferFillRect(&fb,3,75,231,160-55,COLOR_BACKGROUND);
	bufferDrawRect(&fb,3,75,231,160-55,COLOR_FOREGROUND);
	bufferDrawRect(&fb,5,77,231-4,160-55-4,COLOR_FOREGROUND);
	bufferPrint(&fb,9,85,msg,COLOR_FOREGROUND,COLOR_FOREGROUND,1,font);
}

// this will ceate the initial board state, io a cross of pegs, with the middle on being empty (=animphase 6)
void InitBoard()
{
	CBoardParts_RemoveAll(BoardParts);
    int X,Y;
    for (Y=0;Y < NrOfRows;Y++)
        for (X=0;X < NrOfCols;X++)
        {
            if ((Y < 3) && (X > 2) && (X < 6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
             if ((Y > 2) && (Y <6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
            if ((Y > 5) && (X > 2) && (X <6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
        }
     CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,4,4), 6);

}

// Checks if we won the game
bool IsWinningGame()
{
    if (PegsLeft() == 1) //must be 1 peg left
    {
        if ((Difficulty == VeryHard) || (Difficulty == Easy))
        {
            if(CPeg_GetAnimPhase(CBoardParts_GetPart(BoardParts,4,4)) < 2) // must be in the middle with veryhard or easy
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}

void GameInit()
{
	CSelector_SetPosition(GameSelector, 4, 4);
	InitBoard();
    Moves = 0;
	playStartSound();
	PrintFormShown = false;
}

// The main Game Loop
void Game()
{
	if (GameState == GSGameInit)
	{
		GameInit();
		GameState -= GSInitDiff;
	}
	bufferDrawImage(&fb,0,0,background_data, background_width, background_height, false, false, false);
	char Msg[100];

	// Write some info to the screen
	sprintf(Msg, "Moves Left:%d", MovesLeft());
	bufferPrint(&fb, 242, 37, Msg, COLOR_FOREGROUND, COLOR_FOREGROUND,1,font);
	
	sprintf(Msg, "Moves:%d", Moves);
	bufferPrint(&fb, 242, 53, Msg, COLOR_FOREGROUND, COLOR_FOREGROUND,1,font);

	sprintf(Msg, "Pegs Left:%d", PegsLeft());
	bufferPrint(&fb, 242, 69, Msg, COLOR_FOREGROUND, COLOR_FOREGROUND,1,font);

	// Only show best pegs if it isn't 0
	if (saveData.BestPegsLeft[Difficulty] != 0)
	{
		sprintf(Msg, "Best Pegs:%d", saveData.BestPegsLeft[Difficulty]);
		bufferPrint(&fb, 242, 85, Msg, COLOR_FOREGROUND, COLOR_FOREGROUND,1,font);
	}
	CBoardParts_Draw(BoardParts);
	CSelector_Draw(GameSelector);

	//need to repeat showing it until it's no longer shown
	if (PrintFormShown)
	{
		// if it's the winning game play the winning sound and show the form with the winning message
		if (IsWinningGame())
		{
			PrintForm("Congrats you have solved the\npuzzle! Try a new difficulty!\n\nPress (B) to continue");
		}
		else // show the loser messager, play loser sound
		{
			PrintForm("You couldn't solve the puzzle!\nDon't give up, try it again!\n\nPress (B) to continue");
		}
	}

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_A_MASK) && ! (prevButtons & BUTTON_A_MASK))
		decVolumeSound();

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_C_MASK) && ! (prevButtons & BUTTON_C_MASK))
		incVolumeSound();

	if(!(currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_A_MASK) && ! (prevButtons & BUTTON_A_MASK))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X - 1, CSelector_GetPosition(GameSelector).Y);

	if(!(currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_C_MASK) && ! (prevButtons & BUTTON_C_MASK))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X + 1, CSelector_GetPosition(GameSelector).Y);

	if((currButtons & BUTTON_UP_MASK) && ! (prevButtons & BUTTON_UP_MASK))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y - 1);

	if((currButtons & BUTTON_DOWN_MASK) && ! (prevButtons & BUTTON_DOWN_MASK))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y + 1);

	if((currButtons & BUTTON_HOME_MASK) && ! (prevButtons & BUTTON_HOME_MASK))
	{
		GameState = GSTitleScreenInit;
		PrintFormShown = false;
		playMenuBackSound();
	}

	if((currButtons & BUTTON_B_MASK) && ! (prevButtons & BUTTON_B_MASK))
	{
		if (PrintFormShown)
		{
			GameState = GSTitleScreenInit;
			PrintFormShown = false;
			playMenuAcknowlege();
		}
		else
		{
			if (GameSelector->HasSelection) // if we have a selection
			{
				// see if the selected boardpart can move to the current position
				if (CPeg_CanMoveTo(CBoardParts_GetPart(BoardParts, CSelector_GetSelection(GameSelector).X, CSelector_GetSelection(GameSelector).Y), CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y, true))
				{
					playGameAction();
					//if so play a sound, increase the moves, set the selected part to empty and the current part to red
					Moves++;
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetSelection(GameSelector).X, CSelector_GetSelection(GameSelector).Y), 6);
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y), 0);
					// if no moves are left see if the best pegs left value for the current difficulty is
					// greater if so set te new value
					if (MovesLeft() == 0)
					{
						if (saveData.BestPegsLeft[Difficulty] != 0)
						{
							if (PegsLeft() < saveData.BestPegsLeft[Difficulty])
								saveData.BestPegsLeft[Difficulty] = PegsLeft();
						}
						else
							saveData.BestPegsLeft[Difficulty] = PegsLeft();
						saveSavedState();
						// if it's the winning game play the winning sound and show the form with the winning message
						if (IsWinningGame())
						{
							playWinnerSound();
							PrintFormShown = true;
						}
						else // show the loser messager, play loser sound
						{
							playLoserSound();
							PrintFormShown = true;
						}
					}

				}
				else // if we can't move to the spot, play the wrong move sound, and reset the selection to a red peg (instead of blue / selected)
				{
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetSelection(GameSelector).X, CSelector_GetSelection(GameSelector).Y), 0);
					playErrorSound();
				}
				CSelector_DeSelect(GameSelector); // deselect the selection
			}
			else // we didn't have a selection, set the new selection
			{
				if (CPeg_GetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y)) == 0)
				{
					playGameAction();
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y), 1);
					CSelector_Select(GameSelector);
				}

			}
		}
	}
}

void TitleScreenInit()
{

}

// main title screen loop
void TitleScreen()
{
	if(GameState == GSTitleScreenInit)
	{
		TitleScreenInit();
		GameState -= GSInitDiff;
	}

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_A_MASK) && ! (prevButtons & BUTTON_A_MASK))
		decVolumeSound();

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_C_MASK) && ! (prevButtons & BUTTON_C_MASK))
		incVolumeSound();

	if((currButtons & BUTTON_DOWN_MASK) && ! (prevButtons & BUTTON_DOWN_MASK))
	{
		CMainMenu_NextItem(Menu);
	}
	
	if((currButtons & BUTTON_UP_MASK) && ! (prevButtons & BUTTON_UP_MASK))
	{
		CMainMenu_PreviousItem(Menu);
	}

	if((currButtons & BUTTON_B_MASK) && ! (prevButtons & BUTTON_B_MASK))
	{
		playMenuAcknowlege();
		switch (CMainMenu_GetSelection(Menu))
		{
			case 1:
				GameState = GSDifficultySelectInit;
				break;
			case 2:
				GameState = GSCreditsInit;
				break;
		}
	}
	CMainMenu_Draw(Menu);
}

void DifficultySelectInit()
{
}

// Main difficulty select loop
void DifficultySelect()
{
	if(GameState == GSDifficultySelectInit)
	{
		DifficultySelectInit();
		GameState -= GSInitDiff;
	}
	
	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_A_MASK) && ! (prevButtons & BUTTON_A_MASK))
		decVolumeSound();

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_C_MASK) && ! (prevButtons & BUTTON_C_MASK))
		incVolumeSound();

	if((currButtons & BUTTON_HOME_MASK) && ! (prevButtons & BUTTON_HOME_MASK))
	{
		GameState = GSTitleScreenInit;
		playMenuBackSound();
	}
	
	if((currButtons & BUTTON_B_MASK) && ! (prevButtons & BUTTON_B_MASK))
	{
		playMenuAcknowlege();
		GameState = GSGameInit;
	}
	
	if(!(currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_A_MASK) && ! (prevButtons & BUTTON_A_MASK))
	{
		playMenuSelectSound();
		if (Difficulty == VeryHard)
		{
			Difficulty = Hard;
		}
		else
			if (Difficulty == Hard)
			{
				Difficulty = Easy;
			}
			else
				if (Difficulty == Easy)
				{
					Difficulty = VeryEasy;
				}
				else
					if (Difficulty == VeryEasy)
						Difficulty = VeryHard;
	}
	
	if(!(currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_C_MASK) && ! (prevButtons & BUTTON_C_MASK))
	{
		playMenuSelectSound();
		if (Difficulty == VeryEasy)
		{
			Difficulty = Easy;
		}
		else
			if (Difficulty == Easy)
			{
				Difficulty = Hard;
			}
			else
				if (Difficulty == Hard)
				{
					Difficulty = VeryHard;
				}
				else
					if (Difficulty == VeryHard)
						Difficulty = VeryEasy;
	}
	
	// decide what we draw to the buffer based on the difficuly
	switch(Difficulty)
	{
		case VeryEasy:
			bufferDrawImage(&fb, 0, 0, infoveryeasy_data, infoveryeasy_width, infoveryeasy_height, false, false, false);
			bufferDrawImageTransparent(&fb, WINDOW_WIDTH / 2 - veryeasy1_width / 2, 35, veryeasy1_data, veryeasy1_width, veryeasy1_height, false, false, false,COLOR_TRANSPARENT);
			break;
		case Easy:
			bufferDrawImage(&fb, 0, 0, infoeasy_data, infoeasy_width, infoeasy_height, false, false, false);
			bufferDrawImageTransparent(&fb, WINDOW_WIDTH / 2 - easy1_width / 2, 35, easy1_data, easy1_width, easy1_height, false, false, false,COLOR_TRANSPARENT);
		break;
		case Hard:
			bufferDrawImage(&fb, 0, 0, infohard_data, infohard_width, infohard_height, false, false, false);
			bufferDrawImageTransparent(&fb, WINDOW_WIDTH / 2 - hard1_width / 2, 35, hard1_data, hard1_width, hard1_height, false, false, false,COLOR_TRANSPARENT);
			break;
		case VeryHard:
			bufferDrawImage(&fb, 0, 0, infoveryhard_data, infoveryhard_width, infoveryhard_height, false, false, false);
			bufferDrawImageTransparent(&fb, WINDOW_WIDTH / 2 - veryhard1_width / 2, 35, veryhard1_data, veryhard1_width, veryhard1_height, false, false, false,COLOR_TRANSPARENT);
			break;

	}
}

void CreditsInit()
{
}

//Main Credits loop, will just show an image and wait for a button to be pressed
void Credits()
{
	if (GameState == GSCreditsInit)
	{
		CreditsInit();
		GameState -= GSInitDiff;
	}

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_A_MASK) && ! (prevButtons & BUTTON_A_MASK))
		decVolumeSound();

	if((currButtons & BUTTON_B_MASK) && (currButtons & BUTTON_C_MASK) && ! (prevButtons & BUTTON_C_MASK))
		incVolumeSound();

	if(((currButtons & BUTTON_B_MASK) && ! (prevButtons & BUTTON_B_MASK)) ||
		((currButtons & BUTTON_HOME_MASK) && ! (prevButtons & BUTTON_HOME_MASK)))
		{
			playMenuAcknowlege();
			GameState = GSTitleScreenInit;
		}
	bufferDrawImage(&fb,0,0,credits_data, credits_width, credits_height, false, false, false);
}

void mainLoop(void)
{
    switch(GameState)
    {
		case GSGameInit:
        case GSGame :
            Game();
            break;
		case GSTitleScreenInit:
        case GSTitleScreen:
            TitleScreen();
            break;
		case GSDifficultySelectInit:
        case GSDifficultySelect:
            DifficultySelect();
            break;
		case GSCreditsInit:
        case GSCredits:
            Credits();
            break;
		default:
			break;
    }
}

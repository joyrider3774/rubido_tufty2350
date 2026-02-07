# Rubido Pimoroni Tufty2350 Version
![DownloadCountTotal](https://img.shields.io/github/downloads/joyrider3774/rubido_tufty2350/total?label=total%20downloads&style=plastic) ![DownloadCountLatest](https://img.shields.io/github/downloads/joyrider3774/rubido_tufty2350/latest/total?style=plastic) ![LatestVersion](https://img.shields.io/github/v/tag/joyrider3774/rubido_tufty2350?label=Latest%20version&style=plastic) ![License](https://img.shields.io/github/license/joyrider3774/rubido_tufty2350?style=plastic)

Rubido is a little chinese checkers or solitaire game with four difficulties.

## Playing the Game:
The aim of the game in chinese checkers is to select a (white) peg on the board and jump over another (white) peg to land on an empty (black) spot. When doing this the peg you jumped over will be removed from the board.
You need to play the game in such a way that only one peg remains on the board at the end. Depending on the difficulty you had chosen this can be either (only) in the middle of the board or anywhere on the board.
Also depending on the difficulty you had chosen you can either jump horizontally and veritically over pegs or diagonally as well.

## Diffuclties 

### Very Easy
- Jump over Pegs vertically, horizontally and diagonally
- Last Peg can be anywhere on the board

### Easy
- Jump over Pegs vertically, horizontally and diagonally
- Last Peg must end on the middle board

### Hard
- Jump over Pegs vertically and horizontally only
- Last Peg can be anywhere on the board

### Very Hard
- Jump over Pegs vertically and horizontally only
- Last Peg must end on the middle board

## Controls

| Button | Action |
| ------ | ------ |
| A | Left in difficulties screen. During gameplay move the peg selector left. |
| C | Right in difficulties screen. During gameplay move the peg selector Right. |
| UP | Up in main menu screen. During gameplay move the peg selector Up. |
| DOWN | Down in main menu screen. During gameplay move the peg selector Down. |
| B | Confirm in menu and difficulty selector. During gameplay activate the peg where the peg selector is. If there was a peg already selected it will deselect it |
| BOOT / HOME | return to titlescreen |

## Credits
Graphcis are made by me willems davy aka joyrider3774 using gimp, based on the colored graphics i had created for the gp2x version, more than a decade ago.
Tufty2350.h board setup, powman.c & powman.h comes from pimoroni repo related to tufty2350

## Notes 
this repo uses a fork of pimoroni-pico where a change has been made to not build the examples but also a change to the old st7789 driver to make it work with the overlocking done on tufty .
rp2350. You could still use the original repo if you disable / comment the overclocking in pimoroni_tufty2350.h in the boards directory. The micropython firmware also runs at 200mhz and the 
pimoroni_tufty2350.h header comes from there hence why it applies the same overlock
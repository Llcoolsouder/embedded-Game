/*
 * lonnie_game.c
 *
 *  Created on: Mar 26, 2019
 *      Author: lonni
 */


#include "project_settings.h"
#include "random_int.h"
#include "stddef.h"
#include "strings.h"
#include "game.h"
#include "timing.h"
#include "task.h"
#include "terminal.h"
#include "random_int.h"

#define MAP_WIDTH                   20
#define MAP_HEIGHT                  60

/// game structure
struct lonnie_game_t {
    uint8_t x; ///< x coordinate of player
    uint8_t y; ///< y coordinate of player
    char c; ///< character of ship
    int score; ///< score for the round
    uint8_t id; ///< ID of game=
};
static struct lonnie_game_t game;

/* note the user doesn't need to access these functions directly so they are
   defined here instead of in the .h file
   further they are made static so that no other files can access them
   ALSO OTHER MODULES CAN USE THESE SAME NAMES SINCE THEY ARE STATIC */
static void Callback(int argc, char * argv[]);
static void Receiver(uint8_t c);

static void Play(void);
static void Help(void);
static void MoveRight(void);
static void MoveLeft(void);
static void MoveUp(void);
static void MoveDown(void);
static void UpdateScore(void);
static void ResetScreenColor(void);
static void IncreaseScore(void);
static void GameOver(void);

void LonnieGame_Init(void)
{
    // Register the module with the game system and give it the name "MUH3"
    game.id = Game_Register("Scrolly Boy", "A vertical platformer, Temple Run knockoff", Play, Help);
    // Register a callback with the game system.
    // this is only needed if your game supports more than "play", "help" and "highscores"
    Game_RegisterCallback(game.id, Callback);
}

void Play(void)
{
    Game_ClearScreen();
    Game_DrawRect(0, 0, MAP_WIDTH, MAP_HEIGHT);
}

void Help(void)
{
    Game_Printf("WASD to move your character\r\n");
    Game_Printf("Obstacles will fall from the top of the screen. Avoid them at all costs.\r\nGood luck, adventurer! ");
}

void Callback(int argc, char * argv[])
{
    // "play" and "help" are called automatically so just process "reset" here
    if(argc == 0) Game_Log(game.id, "too few args");
    if(strcasecmp(argv[0],"reset") == 0) {
        // reset scores
        game.score = 0;
        Game_Log(game.id, "Scores reset");
    }
    else Game_Log(game.id, "command not supported");
}

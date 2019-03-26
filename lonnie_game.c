/*
 * lonnie_game.c
 *
 *  Created on: Mar 26, 2019
 *      Author: lonni
 */

#include "lonnie_game.h"

#include "project_settings.h"
#include <stdint.h>
#include "random_int.h"
#include "stddef.h"
#include "strings.h"
#include "game.h"
#include "timing.h"
#include "task.h"
#include "terminal.h"


#define MAP_WIDTH                   20
#define MAP_HEIGHT                  30

/// game structure
struct lonnie_game_t {
    uint8_t player_x; ///< x coordinate of player
    uint8_t player_y; ///< y coordinate of player
    char c; ///< character of player
    char c_obstacle;
    char c_boom;
    int score; ///< score for the round
    uint8_t id; ///< ID of game=
};
static struct lonnie_game_t game;

static uint8_t obstacles[MAP_WIDTH][MAP_HEIGHT];

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
//static void MoveUp(void);
//static void MoveDown(void);
static void ShiftObstaclesDown(void);
static void GenerateObstacle(void);
static void UpdateScore(void);
//static void ResetScreenColor(void);
static void GameOver(void);

void LonnieGame_Init(void)
{
    // Register the module with the game system and give it the name "MUH3"
    game.id = Game_Register("ScrollyBoy", "A vertical platformer, Temple Run knockoff", Play, Help);
    // Register a callback with the game system.
    // this is only needed if your game supports more than "play", "help" and "highscores"
    Game_RegisterCallback(game.id, Callback);
}

void Play(void)
{
    Game_ClearScreen();
    Game_DrawRect(0, 0, MAP_WIDTH, MAP_HEIGHT);
	
    volatile int i, j;
	// Init screen objects
	for (i = 0; i < MAP_WIDTH; i++)
	{
		for (j = 0; j < MAP_HEIGHT; j++)
		{
			obstacles[i][j] = 0;
		}
	}
	
	// Init game vals
	game.player_x = MAP_WIDTH/2;
	game.player_y = MAP_HEIGHT-1;
	game.c = 'y';
	game.c_obstacle = 'O';
	game.c_boom = '*';
	game.score = 0;
	
	// Draw player
	Game_SetColor(ForegroundCyan);
	Game_CharXY(game.c, game.player_x, game.player_y);
	Game_SetColor(ForegroundWhite);
	Game_RegisterPlayer1Receiver(Receiver);	// Write Receiver() next

	Game_HideCursor();

	Task_Queue((task_t)UpdateScore, 0);
	Task_Schedule(ShiftObstaclesDown, 0, 250, 250);
	Task_Schedule(GenerateObstacle, 0, 500, 500);
}

void Help(void)
{
    Game_Printf("WASD to move your character\r\n");
    Game_Printf("Obstacles will fall from the top of the screen. Avoid them at all costs.\r\nGood luck, adventurer!\r\n");
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

void Receiver(uint8_t c)
{
    switch (c)
    {
    case 'a':
    case 'A':
        MoveLeft();
        break;
    case 'd':
    case 'D':
        MoveRight();
        break;
    case 'w':
    case 'W':
        //MoveUp();
        break;
    case 's':
    case 'S':
        //MoveDown();
        break;
    default:
        break;
    }
}

void MoveRight()
{
    // Check right border
    if (game.player_x < MAP_WIDTH - 2)
    {
        /* Clear location and update */
        Game_CharXY(' ', game.player_x, game.player_y);
        game.player_x++;
        Game_CharXY(game.c, game.player_x, game.player_y);
    }
}

void MoveLeft()
{
    // Check left border
    if (game.player_x > 2)
    {
        /* Clear location and update */
        Game_CharXY(' ', game.player_x, game.player_y);
        game.player_x--;
        Game_CharXY(game.c, game.player_x, game.player_y);
    }
}

void GenerateObstacle(void)
{
    uint8_t xpos = random_int(2, MAP_WIDTH-2);
    // Always set obstacle at the top
    obstacles[xpos][2] = game.c_obstacle;
    Game_CharXY(game.c_obstacle, xpos, 2);
}

void ShiftObstaclesDown(void)
{
    volatile uint8_t x;
    volatile uint8_t y;
    for (y = MAP_HEIGHT - 1; y >=2; y--)
    {
        for (x=2; x <= MAP_WIDTH-1; x++)
        {
            if (obstacles[x][y] == game.c_obstacle)
            {
                // Clear this object
                obstacles[x][y] = 0;
                Game_CharXY(' ', x, y);

                // Obstacle is NOT already at the bottom
                if (y <= MAP_HEIGHT - 2)
                {
                    obstacles[x][y+1] = game.c_obstacle;

                    // Player collsion
                    if (x == game.player_x && y+1 == game.player_y)
                    {
                        Game_CharXY(game.c_boom, x, y+1);
                        GameOver();
                    }
                    else
                    {
                        Game_CharXY(game.c_obstacle, x, y+1);
                    }
                }
                // Up points for avoiding obstacle
                else
                {
                    game.score++;
                    Task_Queue((task_t)UpdateScore, 0);
                }
            }
        }
    }
}



void GameOver(void) {
    Task_Remove(GenerateObstacle, 0);
    Task_Remove(ShiftObstaclesDown, 0);

    Game_SetColor(ForegroundRed);
    Game_CharXY('\r', 0, MAP_HEIGHT + 1);
    Game_Printf("Game Over! Final score: %d", game.score);
    // unregister the receiver used to run the game
    Game_UnregisterPlayer1Receiver(Receiver);
    // show cursor (it was hidden at the beginning
    Game_CharXY('\r', 0, MAP_HEIGHT + 5);
    Game_ShowCursor();
    Game_SetColor(ForegroundWhite);
    Game_GameOver();
}

void UpdateScore(void) {
    /* Set cursor below the game view and show score */
    Game_CharXY('\r\n', 0, MAP_HEIGHT + 1);
    Game_Printf("Score: %d", game.score);
}

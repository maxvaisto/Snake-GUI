/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: GUI_Constants.hh                                           #
# Description: Declares a class representing the gameboard.        #
#                                                                  #
# Author: Max Väistö, K440046, max.vaisto@tuni.fi                  #
####################################################################
*
* The file has been altered by adding a way to get the snake length,
* snake's head's Point, and food Point.
*
* The gameboard is a class that is used to run the snake game and
* the functions required to operate it. The gameboard class has the
* variables food and snake vector which consist of Points (introduced
* in points.hh further). The allowed points for the food and the snake
* are defined by the width_ and heigth_ which are required as input
* when a gameboard is created.
*
* The functions that I've created are at the bottom of the public
* functions list. The function get_snake_head_point could have been
* left out by making the get_head function public but because of the
* differing type of error printing, I've decided to leave get_head
* as a private function instead and the mainwindow uses the
* get_snake_head_point instead
*
*
*/
#ifndef GUI_CONSTANTS_HH
#define GUI_CONSTANTS_HH
#include <QtGui>
#include "point.hh"
#include "gameboard.hh"

//Game debug messages and string
const QString GAME_LOST_MESSAGE = "You lost!"; //Game lost message
const QString GAME_WON_MESSAGE = "You won!"; // Game won message
//Play push button text when game is not started or
const QString PUSH_BUTTON_PLAY_TEXT = "Play";
//Play push button text when game is playing
const QString PUSH_BUTTON_PAUSE_TEXT = "Pause";
//Game lost debug message
const QString GAME_LOST_DEBUG_MESSAGE = "Game lost!";
//Game won debug message
const QString GAME_WON_DEBUG_MESSAGE = "Game won!";
//Replace replace_head_with_body function debug tag
const QString REPLACE_HEAD_DEBUG_TAG = "Head replacement";
//Replace replace_snake_end_with_tail function debug tag
const QString REPLACE_TAIL_DEBUG_TAG = "Tail replacement";
const QString NO_SNAKE_ERROR_MESSAGE = "Error: Cannot replace empty "
                                            "snake vector head tile";
//The snake_tiles vector is empty error message
const QString EMPTY_SNAKE_ERROR_MESSAGE = "Error: Snake is empty.";
//Debug tag and message separator
const QString TAG_SEPARARTOR = ": ";
//new snake created debug message
const QString SNAKE_CREATED_DEBUG_MESSAGE = "New snake created";
//game_info_text_browser text when game is paused
const QString GAME_PAUSED_MESSAGE = "Game paused";
//Game paused debug message
const QString GAME_PAUSED_DEBUG_MESSAGE = "Game paused";
//game_info_text_browser text when game is unpaused
const QString GAME_UNPAUSED_DEBUG_MESSAGE = "Game Unpaused";
//Game score increased debug message
const QString SCORE_INCREASE_DEBUG_MESSAGE = "Score increased";
//Error message for when constants are to be set but the scene does not exist yet
const QString SET_CONSTANTS_ERROR_MESSAGE = "Error: Cannot set constants: Scene does not exist";


//Snake direction controlling keys
const Qt::Key MOVE_LEFT_KEY = Qt::Key_A;
const Qt::Key MOVE_RIGHT_KEY = Qt::Key_D;
const Qt::Key MOVE_UP_KEY = Qt::Key_W;
const Qt::Key MOVE_DOWN_KEY  = Qt::Key_S;

//Gameboard default values
const unsigned int MIN_GAMEBOARD_DIMENSION = 3;
const unsigned int MAX_GAMEBOARD_DIMENSION = 10;
//The ammount of wall tiles on each side of the gameboard
const unsigned int GAMEBOARD_WALL_THICKNESS = 1;
const unsigned int TICK_DURATION  = 1111; //Game tick length
const unsigned int SECOND_IN_MILLISECEONDS = 1000; //GUI clock tick length


const int MAX_RGB_VALUE = 255;
const int MIN_RGB_VALUE = 0;
const int FOOD_SCORE_VALUE = 20;
const int COLOR_RGB_OFFSET_ = -10;
const int DEFAULT_GAME_TIME_ = 0;
const int DEFAULT_GAME_SCORE = 0;
const Point DEFAULT_POINT_VALUE = Point(-1,-1);

//Snake defaults
const int NO_SNAKE = 0;
const int MIN_TAILED_SNAKE_LENGTH = 2;
const int MIN_BODYED_SNAKE_LENGTH = 3;

//The relative sizes of snake parts
//The special snake parts are compared to the snake body size
//while the snake body and food is in comparison to the background tiles
const double snake_body_to_background_tile_ratio = 0.7;
const double snake_head_to_body_tile_ratio = 1.1;
const double snake_tail_to_body_tile_ratio = 0.7;
const double food_to_background_tile_ratio = 0.6;

//Default start direction of movement
const char DEFAULT_NEXT_DIRECTION = DOWN;
const char DEFAULT_PREVIOUS_DIRECTION = UP;


#endif // GUI_CONSTANTS_HH

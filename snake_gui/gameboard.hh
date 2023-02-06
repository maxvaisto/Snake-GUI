/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: gameboard.hh                                               #
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

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH


#include <random>
#include <string>
#include <deque> // Better than vector, since we need erase front elements
#include <QDebug>
#include "point.hh"

// Constants for printing the gameboard
const std::string BODY = "~";
const std::string DEAD = "X";
const std::string FOOD = "*";
const std::string HEAD = "@";
const std::string TAIL = "-";
const std::string WALL = "#";
const std::string EMPTY = " ";

const int DEFAULT_WIDTH = 5;
const int DEFAULT_HEIGTH = 6;
const int DEFAULT_SEED = 1;

class GameBoard {
public:
    // Default constructor.
    // Calls the other constructor with default width, height, and seed.
    GameBoard();

    // Constructs a gameboard based the given width and height,
    // rng is used to generate random locations for the food.
    GameBoard(int width, int height, int seed_value);

    // Destructor
    ~GameBoard();

    // Checks if the game is over.
    // Returns false until the game has been either lost or won.
    bool game_over() const;

    // Checks if the game has been lost.
    // Returns true if the snake is dead.
    bool game_lost() const;

    // Checks if the game has been won.
    // Returns true if the snake has filled the field.
    bool game_won() const;

    // Moves the snake to the given direction, if possible.
    // Moving is not possible, if game is over, or if the snake would move
    // against a wall.
    // If moving is possible, calls the private method moveSnakeAndFood.
    // Returns true, if moving was possible.
    bool move_snake(const std::string& direction);

    // Prints the gameboard and the snake in it.
    void print() const;

    //OWN ADDITIONS

    //This is used get a Point value of the snake tile coordinates
    //The function returns the snake point at the index of index
    //If the index is invalid the functions returns the point Point(-1,-1)
    //and prints a error into qDebug()
    Point get_snake_head_point();

    //This returns the size of the snake
    //If the snake is empty the size returned is zero
    int snake_length();

    //This function is used to the get the location of the food_ entity
    //the function returns the food_ entity's Point value
    Point get_food_point() const;

private:
    // Returns true if the given point is a part of the snake.
    bool is_snake_point(const Point& point) const;

    // Returns the point of the snake's head.
    const Point& get_head() const;

    // Returns the point of the snake's tail.
    const Point& get_tail() const;

    // Relocates food to a random, snakeless location.
    // Hides the food if the snake has completely filled the gameboard.
    void move_food();

    // Moves the snake unless the new head would be the body of the snake.
    // If the new head is the neck of the snake, nothing happens.
    // If the new head is other body part of the snake, the snake dies.
    // Otherwise the snake moves, whereupon the method returns true.
    // If, in addition, the food got eaten a new one is placed somewhere,
    // and the snake grows a bit.
    bool move_snake_and_food(const Point& new_head);

    // Prints the top or bottom wall of the gameboard.
    void print_horizontal_wall() const;

    // Tells if the snake is alive and able to move.
    bool dead_ = false;

    // Specifies the width and height of the gameboard.
    const int width_ = 0;
    const int height_ = 0;

    // Generates random numbers used to move the food item to random locations.
    std::default_random_engine rng_;

    // Points currently occupied by the snake, head being the last one.
    std::deque<Point> snake_;

    // The food item's position in the gameboard.
    Point food_;
};  // class GameBoard


#endif  // GAMEBOARD_HH

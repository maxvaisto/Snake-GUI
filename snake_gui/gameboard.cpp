/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: gameboard.cpp                                              #
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

#include "gameboard.hh"
#include <iostream>

GameBoard::GameBoard() : GameBoard(DEFAULT_WIDTH, DEFAULT_HEIGTH, DEFAULT_SEED)
{
}

GameBoard::GameBoard(int width, int height, int seed_value):
    width_(width), height_(height)
{
    rng_.seed(seed_value);

    // Creating a snake whose head is about in the middle of the gameboard
    const Point head((width_ - 1) / 2, (height_ - 1) / 2);
    snake_.push_back(head);

    // Putting food somewhere
    move_food();
}

GameBoard::~GameBoard()
{
}

bool GameBoard::game_over() const
{
    return game_lost() or game_won();
}

bool GameBoard::game_lost() const
{
    return dead_;
}

bool GameBoard::game_won() const
{
    return static_cast<int>(snake_.size()) >= width_ * height_;
}

bool GameBoard::move_snake(const std::string& direction)
{
    // If the snake can't / doesn't need to move, do nothing
    if (game_over())
    {
        return false;
    }

    // Figuring out the destination coordinates
    Point new_head = get_head();
    new_head.move(direction);

    // Checking if the snake died due to running against a wall
    if(not new_head.is_inside(0, 0, width_ - 1, height_ - 1))
    {
        dead_ = true;
        return false;
    }

    // Actually moving the snake
    return move_snake_and_food(new_head);
}

void GameBoard::print() const
{
    // Printing the top wall
    print_horizontal_wall();

    // Printing rows with playable area
    for (int row = 0; row < height_; ++row)
    {
        std::cout << WALL;
        for (int col = 0; col < width_; ++col)
        {
            const Point position(col, row);
            if (position == food_)
            {
                std::cout << FOOD;
            }
            else if (not is_snake_point(position))
            {
                std::cout << EMPTY;
            }
            else if (game_lost())
            {
                std::cout << DEAD;
            }
            else if (position == get_head())
            {
                std::cout << HEAD;
            }
            else if (position == get_tail())
            {
                std::cout << TAIL;
            }
            else
            {
                std::cout << BODY;
            }
        }
        std::cout << WALL << std::endl;
    }

    // Printing the bottom wall
    print_horizontal_wall();
}


//This is used to get the snake head tile Point
//If the snake is empty the functions returns the point Point(-1,-1)
//and prints a error into qDebug()
//This is the same as the getHead but the getHead function
//does not use qDebug to infor of the error.
Point GameBoard::get_snake_head_point(){
    if (not snake_.empty()){
        return snake_.back();
    } else {
        qDebug() << "Error: Cannot get head Point of empty snake";
        return Point(-1,-1);
    }
}

//This returns the size of the snake
//If the snake is empty the size returned is zero
int GameBoard::snake_length() {
    //Check if snake is empty before getting its length
    if (not snake_.empty()){
         return snake_.size();
    } else {
        qDebug() << "Error: Snake is empty and has no length";
        return 0;
    }

}
//This function is used to the get the location of the food_ entity
//the function returns the food_ entity's Point value
Point GameBoard::get_food_point() const {
    return food_;
}
bool GameBoard::is_snake_point(const Point& point) const
{
    // Checking if any of the Points stored in snake_ match the given point
    for(const Point& part : snake_)
    {
        if (part == point)
        {
            return true;
        }
    }
    return false;
}

const Point& GameBoard::get_head() const
{
    // This should never happen: snake_ contains at least one element (head)
    if(snake_.size() == 0)
    {
        std::cout << "Internal error: empty snake" << std::endl;
    }
    // The snake's head is the last one in the deque
    return snake_.back();
}

const Point& GameBoard::get_tail() const
{
    // This should never happen: snake_ contains at least one element (head)
    if(snake_.size() == 0)
    {
        std::cout << "Internal error: empty snake" << std::endl;
    }
    // The snake's tail is the first one in the deque
    return snake_.front();
}

void GameBoard::move_food()
{
    // Moving food out of sight when it's no longer needed
    if (game_won())
    {
        food_.set_position(-1, -1);
        return;
    }

    // Keeping to try random points until an empty square is found
    std::uniform_int_distribution<int> width_dist(0, width_ - 1);
    std::uniform_int_distribution<int> height_dist(0, height_ - 1);
    width_dist(rng_);
    height_dist(rng_);
    while (true)
    {
        food_.set_position(width_dist(rng_), height_dist(rng_));
        if (not is_snake_point(food_))
        {
            // Snakeless point found, stop moving the food around
            return;
        }
    }
}

bool GameBoard::move_snake_and_food(const Point& new_head)
{
    // There shouldn't be any problems if the snake is only a head or
    // if it doesn't yet occupy the point it's moving into.
    if (snake_.size() > 1 and is_snake_point(new_head))
    {
        const Point& neck = snake_.at(snake_.size() - 2);
        if (new_head == neck)
        {
            // If the destination point is the point before the head,
            // don't move but don't die either
            return false;

        }
        else if (new_head != get_tail())
        {
            // If the destination point is the snake's body point, other than
            // "neck" or tail, the snake dies
            dead_ = true;
            return false;
        }
    }

    // Moving the snake
    snake_.push_back(new_head);

    // New food must be placed somewhere once one gets eaten.
    // Also, the snake should stretch a little.
    if (new_head == food_)
    {
        move_food();
    }
    else // Otherwise the point is of the old tail becomes empty.
    {
        snake_.pop_front();
    }
    return true;
}

void GameBoard::print_horizontal_wall() const
{
    // Printing a long enough wall to cover the GameBoard and
    // the walls at the sides
    for (int i = 0; i < width_ + 2; ++i)
    {
        std::cout << WALL;
    }
    std::cout << std::endl;
}

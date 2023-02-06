/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: point.hh                                                   #
# Description: Declares a class representing a point in            #
# the gameboard.                                                   #
#                                                                  #
# Author: Max Väistö, K440046, max.vaisto@tuni.fi                  #
####################################################################
* Function names changed to match function naming in the other files
* and function position has been added to retrieve the x_ and y_
* values of the point.
*
* Each Point has a x and y integer values which always exists but chan
* be changed with the public functions of the class.
* This can be done via giving it a direction in which the point will
* moved towards or by setting a new position for the point by
* giving it its new x and y values.
*
* This is the header file of the Point class which introduces the
* public and private functions and variables of the Point class.
*/

#include "point.hh"
#include <iostream>

Point::Point() : x_(0), y_(0)
{
}

Point::Point(int x, int y) :
    x_(x), y_(y)
{
}

Point::~Point()
{
}

bool Point::operator==(const Point &rhs) const
{
    return x_ == rhs.x_ and y_ == rhs.y_;
}

bool Point::operator!=(const Point &rhs) const
{
    return x_ != rhs.x_ or y_ != rhs.y_;
}

void Point::set_position(int x, int y)
{
    x_ = x;
    y_ = y;
}

void Point::move(const std::string &dir)
{
    if(dir.size() != 1)
    {
        std::cout << "Internal error: impossible direction" << std::endl;
        return;
    }
    switch(dir.at(0))
    {
        case UP: --y_; break;
        case LEFT: --x_; break;
        case DOWN: ++y_; break;
        case RIGHT: ++x_; break;
        default: std::cout << "Internal error: impossible direction"
                           << std::endl;
    }
}

bool Point::is_inside(int left_top_x, int left_top_y,
                     int right_bottom_x, int right_bottom_y) const
{
    return (x_ >= left_top_x) and (y_ >= left_top_y) and
           (x_ <= right_bottom_x) and (y_ <= right_bottom_y);
}

//Returns the pair of the x_ and y_ values
std::pair<int,int> Point::position() const{
    return std::make_pair(x_,y_);
}

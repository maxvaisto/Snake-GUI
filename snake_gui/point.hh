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

#ifndef POINT_HH
#define POINT_HH

#include <string>

// Constants for directions
const char UP = 'w';
const char LEFT = 'a';
const char DOWN = 's';
const char RIGHT = 'd';

class Point
{
public:
    // Default constructor, creates a new point at (0, 0).
    Point();

    // Constructor, creates a new point at the given coordinates.
    Point(int x, int y);

    // Destructor
    ~Point();

    // Comparison operators
    bool operator==(const Point& rhs) const;
    bool operator!=(const Point& rhs) const;

    // Sets a new position for the point
    void set_position(int x, int y);

    // Moves the point into the given direction, direction can be one of
    // constants above.
    void move(const std::string& dir);

    // Tells if the point is inside the square specified by the given
    // parameters.
    bool is_inside(int left_top_x, int left_top_y,
                  int right_bottom_x, int right_bottom_y) const;

    //This is used to get the x_ and y_ values of the Point
    std::pair<int,int> position() const;

private:
    // Coordinates of the point
    int x_;
    int y_;
};

#endif // POINT_HH

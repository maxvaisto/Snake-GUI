/*
####################################################################
# COMP.CS.110 Programming 2: Structures, 2022                      #
#                                                                  #
# Project4: Snake                                                  #
# Program description: Implements a game called Snake.             #
#                                                                  #
# File: GUI_Constants.cpp                                          #
# Description: Implements all of the mainwindow.hh functions.      #
#                                                                  #
# Author: Max Väistö, K440046, max.vaisto@tuni.fi                  #
####################################################################
*
*/

//Information about the game is in the mainwindow source file
//with the details about how the program functions.

#include "mainwindow.hh"
#include "ui_mainwindow.h"

//Sets up the GUI for the mainwindow as well as sets
//the pallets for the brushes and pens used in the
//QGraphicsScene
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set spinbox limits and default value
    ui->height_spin_box->setMinimum(MIN_GAMEBOARD_DIMENSION);
    ui->width_spin_box->setMinimum(MIN_GAMEBOARD_DIMENSION);
    ui->height_spin_box->setValue(DEFAULT_HEIGTH);
    ui->width_spin_box->setValue(DEFAULT_WIDTH);
    ui->height_spin_box->setMaximum(MAX_GAMEBOARD_DIMENSION);
    ui->width_spin_box->setMaximum(MAX_GAMEBOARD_DIMENSION);
    ui->seed_spin_box->setValue(DEFAULT_SEED);

    //Create the time that updates the gameboard
    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::update_game);
    ui->game_info_text_browser->setTextBackgroundColor(Qt::transparent);

    //Create the timer that works as the game length clock
    //This increases the GUI game clock.
    clock_.setSingleShot(false);
    connect(&clock_,&QTimer::timeout, this, &MainWindow::clock_tick);

    //Set font for game_info_text_browser
    ui->game_info_text_browser->setStyleSheet("background: transparent;");
    ui->game_info_text_browser->setAlignment(Qt::AlignCenter);
    ui->game_info_text_browser->setTextColor(QColorConstants::Svg::slateblue);
    QFont game_info_font = ui->game_info_text_browser->font();
    game_info_font.setBold(true);
    game_info_font.setPointSize(50);
    game_info_font.setCapitalization(QFont::AllUppercase);
    ui->game_info_text_browser->setFont(game_info_font);

    //Set centralwidget background color
    this->setPalette(Qt::black);


    //Set tile QBrushes and Pen colors

    //  Background and walls
    background_wall_brush_.setStyle(Qt::SolidPattern);
    background_wall_brush_.setColor(Qt::black);
    background_tile_odd_brush_.setStyle(Qt::SolidPattern);
    background_tile_even_brush_.setStyle(Qt::SolidPattern);
    background_tile_odd_brush_.setColor(QColorConstants::Svg::chartreuse);
    background_tile_even_brush_.setColor(QColorConstants::Svg::forestgreen);
    background_wall_pen.setColor(Qt::black);

    //  Snake
    snake_brush_.setColor(QColorConstants::Svg::deepskyblue);
    snake_pen_.setColor(QColorConstants::Svg::deepskyblue);
    snake_brush_.setStyle(Qt::SolidPattern);

    // Food
    food_brush_.setColor(QColorConstants::Svg::orangered);
    food_pen_.setColor(QColorConstants::Svg::orangered);
    food_brush_.setStyle(Qt::SolidPattern);



}

//Called when the mainwindow is closed
//deletes all remaining objects
MainWindow::~MainWindow()
{

    delete gameboard_;
    clear_scene();
    //delete scene;
    delete ui;

}

//This is used to monitor the clicks regarding the start button
//The start button has two modes:
//(1) Game exists -> Pause / Play and toggle timers
//(2) Game doesnt exist -> Create new game
void MainWindow::on_start_push_button_clicked()
{
    if (game_exists_){
        if (game_is_running_){

            qDebug() << GAME_PAUSED_DEBUG_MESSAGE;
            change_info_text(GAME_PAUSED_MESSAGE);
            ui->start_push_button->setText(PUSH_BUTTON_PLAY_TEXT);
            ui->game_info_text_browser->setVisible(true);
        } else {
            qDebug() << GAME_UNPAUSED_DEBUG_MESSAGE;

            ui->start_push_button->setText(PUSH_BUTTON_PAUSE_TEXT);
            ui->game_info_text_browser->setVisible(false);
        }
        //Toggle the timers
        toggle_timer_run_status();


    } else {
        create_new_game();
        qDebug() << "New gameboard created:" << gameboard_;
        ui->start_push_button->setText(PUSH_BUTTON_PAUSE_TEXT);
    }

}

//Called if the user presses the reset button which will create a new game
void MainWindow::on_reset_push_button_clicked()
{
    create_new_game();
}

//Used to monitor snake movement related keypresses
//If such input is received while the game is running
//without it being the opposite direction of the last snake direction,
//the next snake movement direction will be set as the input direction.
void MainWindow::keyPressEvent(QKeyEvent* event){


    if(game_is_running_){
        //Left if left key and last move was not to right
        if (event->key() == MOVE_LEFT_KEY
                && last_direction_.at(0) != RIGHT ){
            next_direction_ = LEFT;
        }
        //Right if right key and last move was not to left
        else if (event->key() == MOVE_RIGHT_KEY
                 && last_direction_.at(0) != LEFT ){
            next_direction_ = RIGHT;
        }
        //Up if up key and last move was not to down
        else if (event->key() == MOVE_UP_KEY
                 && last_direction_.at(0) != DOWN){
            next_direction_ = UP;
        }
        //Down if down key and last move was not to up
        else if (event->key() == MOVE_DOWN_KEY
                 && last_direction_ .at(0) != UP){
            next_direction_ = DOWN;

        }
    }
}

//This is called by the game duration clock
//which increases the game time by a second
//and updates the GUI to match this change
void MainWindow::clock_tick() {
    //The ammount of seconds added is relational
    //to the clock tick speed
    game_time_ += SECOND_IN_MILLISECEONDS/1000;
    update_gui_timer();
}

//This is used to update the GUI game length clock
void MainWindow::update_gui_timer() {

    ui->timer_text_browser->setText(QString::number(game_time_));
}

//Called by the game speed clock
//Continues to move and to render the snake as long as the game is not over
//and if the game is over the function will call the game_over function
void MainWindow::update_game(){


     if(gameboard_->game_over()){
        game_over();
     } else {

        gameboard_->move_snake(next_direction_);
        last_direction_ = next_direction_;
        update_snake();
     }
}

//This is used to pause and to continute game clocks
//There are two because one is for the game time count clock
//and the other is for the game tick timer
void MainWindow::toggle_timer_run_status(){
    //The game is running status could have been changed
    //elsewhere so the check here depends on the game tick timer
    if (timer_.isActive()) {
        timer_.stop();
        clock_.stop();
    } else {
        timer_.start();
        clock_.start();
    }
    game_is_running_ = not game_is_running_;
}

//Function init_scene is used to draw the gameboard which
//erases the old graphics_view scene and draws the gameboard onto it.
//init_scene will call on the update_snake and draw_food functions
//to draw these to the empty GUI gameboard.
//This function also erases the previous gameboard and
void MainWindow::init_scene(){

    scene_ = new QGraphicsScene(this);
    ui->graphics_view->setScene(scene_);


    //Let us set the scene coordinate rectangle offsets
    //to match the current board
    set_rectangle_variables();


    QRect rcontent = ui->graphics_view->contentsRect();
    scene_->setSceneRect(0,0,rcontent.width(),rcontent.height());

    //gameboard drawing maximum dimensions are larger than
    //the gameboard due to the gameboard walls
    //There are walls on the both sides so the wall thickness
    //is multiplied by two
    int unsigned max_rows = height_+GAMEBOARD_WALL_THICKNESS*2;
    int unsigned max_columns = width_+GAMEBOARD_WALL_THICKNESS*2;

    QGraphicsRectItem* wall_tile = nullptr;
    for(unsigned int i = 0; i < max_rows;++i){
        for(unsigned int j = 0; j < max_columns;++j){
            int tile_x_coordinates = tile_size_.rwidth()*j;
            int tile_y_coordinates = tile_size_.rheight()*i;


            //Check if wall tile
            //these need to be collected into a vector
            //so that we can change their coloring in the case of an victory
            //or defeat.
            if (i<=GAMEBOARD_WALL_THICKNESS-1
                    || i >= max_rows - GAMEBOARD_WALL_THICKNESS) {
                //If horizontal border wall
                wall_tile = scene_->addRect(tile_x_coordinates,
                                            tile_y_coordinates,
                                            tile_size_.rwidth(),
                                            tile_size_.rheight(),
                                            background_wall_pen,
                                            background_wall_brush_);
                wall_tiles.push_back(wall_tile);
            } else if (j<=GAMEBOARD_WALL_THICKNESS-1
                       || j >= max_columns - GAMEBOARD_WALL_THICKNESS) {
                //If vertical border wall
                wall_tile = scene_->addRect(tile_x_coordinates,
                                           tile_y_coordinates,
                                           tile_size_.rwidth(),
                                           tile_size_.rheight(),
                                           background_wall_pen,
                                           background_wall_brush_);
                wall_tiles.push_back(wall_tile);
            } else {
                //If background tile

                //The background tiles should have a chessboard look
                //which can be created by finding out if the modulo of the
                //sum of the x and y gameboard coordinates is even or odd
                //and to select the tile color based on this value
                if((i+j) % 2 == 0 ){
                    //
                    scene_->addRect(tile_x_coordinates,
                                   tile_y_coordinates,
                                   tile_size_.rwidth()
                                   ,tile_size_.rheight(),
                                   background_tile_even_pen_,
                                   background_tile_even_brush_);
                } else {
                    scene_->addRect(tile_x_coordinates,
                                   tile_y_coordinates,
                                   tile_size_.rwidth(),
                                   tile_size_.rheight(),
                                   background_tile_odd_pen_,
                                   background_tile_odd_brush_);
                }

            }
        }
    }

    //Also draw the food and the snake at the start of the snake game
    draw_food();
    update_snake();
}

//Adds snake parts which vary in location and size depending on the
//type given in the parameters (isHead)
//point pararmeter is used to receive the (rough) coordinates
//of the new snake tile
void MainWindow::add_snake_head(){

    Point snake_head_point = gameboard_->get_snake_head_point();

    //The [direction]_axis_position is used to give the coordinates
    //to the leftmost corner of the retangle
    int x_axis_position = tile_size_.rwidth()
            *(snake_head_point.position().first + GAMEBOARD_WALL_THICKNESS)
            + snake_head_tile_x_offset_;
    int y_axis_position = tile_size_.rheight()
            *(snake_head_point.position().second + GAMEBOARD_WALL_THICKNESS)
            + snake_head_tile_y_offset_;
    //Add the new head to the snake tile vector
    snake_tiles.push_back(scene_->addRect(x_axis_position,y_axis_position,
                       snake_head_tile_width_,snake_head_tile_height_,
                       snake_pen_,snake_brush_));
}

//This is used to change replace the head of the snake with a snake body tile
//The function will
// (1) Get the position of the head tile and calculate
//  the position of the new tile with it
// (2) Remove the old head tile
// (3) Add new tile to the end of snake_tiles
void MainWindow::replace_head_with_body() {

    QGraphicsRectItem* snake_head_pointer = nullptr;

    if(not snake_tiles.empty()){
        snake_head_pointer = snake_tiles.back();


        //Get new coordinates for the rectangle by
        //reverse engineering the tile top corner position
        //and adding the snake tail tile offset
        int x_axis_position = snake_head_pointer->rect().x()
                - (tile_size_.width()- snake_head_pointer->rect().width())/2
                + snake_body_tile_x_offset_;
        int y_axis_position = snake_head_pointer->rect().y()
                - (tile_size_.height()- snake_head_pointer->rect().height())/2
                + snake_body_tile_y_offset_;

        //Delete old head
        scene_->removeItem(snake_head_pointer);
        snake_tiles.pop_back();
        delete snake_head_pointer;

        //Add new body tile to where the head was previously
        snake_tiles.push_back(scene_->addRect(x_axis_position,y_axis_position,
                                        snake_tile_width_,snake_tile_height_,
                                        snake_pen_,snake_brush_));

    } else{
        qDebug() << REPLACE_HEAD_DEBUG_TAG << TAG_SEPARARTOR
                 << NO_SNAKE_ERROR_MESSAGE;
    }


}

//This is used to change replace the last of the snake with a snake tail tile
//The function will
// (1) Get the position of the last tile and calculate
//  the position of the new tile
// (2) Replace and delete the old end tile with the new tail tile
void MainWindow::replace_snake_end_with_tail() {

    QGraphicsRectItem* snake_end_pointer = nullptr;

    if(not snake_tiles.empty()){
        snake_end_pointer = snake_tiles.front();

        //Get the colors of the tile
        QBrush body_brush = snake_end_pointer->brush();
        QPen body_pen = snake_end_pointer->pen();

        //Get new coordinates for the rectangle by
        //reverse engineering the tile top corner position
        //and adding the snake body tile offset
        int x_axis_position = snake_end_pointer->rect().x()
                - (tile_size_.width()- snake_end_pointer->rect().width())/2
                + snake_tail_tile_x_offset_;
        int y_axis_position = snake_end_pointer->rect().y()
                - (tile_size_.height()- snake_end_pointer->rect().height())/2
                + snake_tail_tile_y_offset_;

        //Delete the old snake end
        scene_->removeItem(snake_end_pointer);
        snake_tiles.erase(snake_tiles.begin());
        delete snake_end_pointer;

        //Replace the old end with a tail tile from the snake_tiles vector
        snake_tiles.insert(snake_tiles.begin(),
                           scene_->addRect(x_axis_position,
                                            y_axis_position,
                                            snake_tail_tile_width_,
                                            snake_tail_tile_height_,
                                            body_pen,body_brush));
    } else {
        qDebug() << REPLACE_TAIL_DEBUG_TAG << TAG_SEPARARTOR
                 << NO_SNAKE_ERROR_MESSAGE;
    }

}

//This is used to remove the old food and to draw a new one
//the function calls the get_food_point function to retreive the
//location of the food point
void MainWindow::draw_food() {


   //Remove the current food only if it exists
   if(food_tile_!=nullptr){
       scene_->items();
       scene_->removeItem(food_tile_);
       delete food_tile_;
   }

   food_point_ = gameboard_->get_food_point();
   //The [direction]_axis_position is used to give the coordinates
   //to the leftmost corner of the retangle
   int x_axis_position = tile_size_.rwidth()
           *(food_point_.position().first
            + GAMEBOARD_WALL_THICKNESS)
            + food_tile_y_offset_;
   int y_axis_position = tile_size_.rheight()
           *(food_point_.position().second
            + GAMEBOARD_WALL_THICKNESS)
            + food_tile_y_offset_;

   //Add food tile to the scene and set the food_tile_ to match the new tile
   food_tile_ = scene_->addRect(x_axis_position,
                               y_axis_position,
                               food_tile_width_,
                               food_tile_height_,
                               food_pen_,food_brush_);

}

//Used to alter the snake tiles of the snake in color
//to give the snake a color that differs on every snake tile
void MainWindow::alter_snake_tile_colors(){

    //Do the changes to every snake tile
    for(int i = snake_tiles.size()-1;i>=0;--i){

        //Copy the current color scheme of the tile
        QBrush tile_brush = snake_tiles.at(i)->brush();
        QPen tile_pen = snake_tiles.at(i)->pen();
        QColor color = tile_brush.color();

        //Only change the color values if the color values
        //are possible RGB values in the range [0,1,2,...,255]
        if(color.red()+COLOR_RGB_OFFSET_ >= MIN_RGB_VALUE
                && color.red()+COLOR_RGB_OFFSET_ <= MAX_RGB_VALUE){
            color.setRed(color.red()+COLOR_RGB_OFFSET_);
        }
        if(color.green()+COLOR_RGB_OFFSET_ >= MIN_RGB_VALUE
                && color.green()+COLOR_RGB_OFFSET_ <= MAX_RGB_VALUE){
            color.setGreen(color.green()+COLOR_RGB_OFFSET_);
        }
        if(color.blue()+COLOR_RGB_OFFSET_ >= MIN_RGB_VALUE
                && color.green()+COLOR_RGB_OFFSET_ <= MAX_RGB_VALUE){
            color.setBlue(color.blue()+COLOR_RGB_OFFSET_);
        }
        //Set the colors to the brush and pen

        tile_brush.setColor(color);
        tile_pen.setColor(color);


        //Apply the new colors to the tile
        snake_tiles.at(i)->setPen(tile_pen);
        snake_tiles.at(i)->setBrush(tile_brush);
    }
}

//Used to change the colors of the walls when the game is won or lost
//The parameter color is the QColor that the brushes and pens of the
//tiles will be set to.
void MainWindow::change_wall_color(QColor color){
    //Apply changes to every tile iteratively
    for(unsigned int i = 0;i<wall_tiles.size();++i){
        wall_tiles.at(i)->setBrush(color);
        wall_tiles.at(i)->setPen(color);
    }
}

//This function is used to set the text for the
//gui element info_text_browser so that the text
//is centered and it has a outline
void MainWindow::change_info_text(QString new_text){

    ui->game_info_text_browser->setText(new_text);
    //Center the text
    ui->game_info_text_browser->setAlignment(Qt::AlignCenter);

}


//This function adds and removes snake tiles
//and also checks if food has been eaten
//First the function creates a snake if it doesnt exist
//If the snake exists the function will
// (1) Check if game has was won or lost and update accordingly
// (2) Update the food rectangle if needed
// (3) Update the snake tail if needed
// (4) Update the snake last body tile (neck)
//  if the snake has one
// (5) Alter the coloring of the snake tiles
// (6) Update the head position
void MainWindow::update_snake(){

    Point snake_head_point;
    //If the snake doesn't exist, create it and exit function
    if (snake_tiles.size()==0){
        add_snake_head();
        qDebug() << SNAKE_CREATED_DEBUG_MESSAGE;
        return;
    }

    //Get snake length
    int length = gameboard_->snake_length();


    //Only update snake position if game is not lost
    if (gameboard_->game_lost()){
        return;
    } //If game is won update the snake head and its neck
    else if(gameboard_->game_won()){
        replace_head_with_body();
        add_snake_head();
        increase_score();
        return;
    } //Game is not over
    else {

        //Get snake head position
        snake_head_point = gameboard_->get_snake_head_point();

        //Check if food is eaten
        if (food_point_ == snake_head_point) {

           increase_score();
           draw_food();

           if(length==MIN_TAILED_SNAKE_LENGTH){
               //If the tail was just added to the snake
               //we need to replace the old snake head with it
               replace_snake_end_with_tail();
           }
        } //If food was not eaten, replace tail
        else {
            QGraphicsItem* removeable = nullptr;
            //Remove the old tail tile
            removeable = snake_tiles.front();
            snake_tiles.erase(snake_tiles.begin());
            scene_->removeItem(removeable);
            delete removeable;

            if(length >= MIN_TAILED_SNAKE_LENGTH) {
                //Replace first snake_tile
                //with a tail tile
                replace_snake_end_with_tail();
            }
        }

        //Replace the old head of the snake with a body tile
        //if the snake has body-tiles (is longer than 2)
        if (length >= MIN_BODYED_SNAKE_LENGTH){
            replace_head_with_body();

        }
        //Now since the snake_tiles only consists of the snake body and tail
        //we need to edit their color values to show more easily which part
        //of the snake is on which tile
        alter_snake_tile_colors();

        //Finally, add the snake head tile
        //to the scene and to the snake_tiles vector
        add_snake_head();
    }

}


//create_new_game function is used to initialize a new game
//The function will in order:
// (1) Erase the old gameboard
// (2) Reset game related variables
// (3) Get gameboard dimensions and creates it and calls
//  for it to be rendered.
// (4) Starts game timers
// (5) Hide items blocking the gameboard in the GUI
void MainWindow::create_new_game(){

    //Delete the last gameboard if it already exists
    if (gameboard_ != nullptr) {
        delete gameboard_;
    }

    //Delete GUI scene tile pointers
    clear_scene();

    //Reset some game state depended values
    game_exists_ = true;
    game_is_running_ = true;
    next_direction_= DEFAULT_NEXT_DIRECTION;
    last_direction_= DEFAULT_PREVIOUS_DIRECTION;
    game_time_ = DEFAULT_GAME_TIME_;
    current_score_ = DEFAULT_GAME_SCORE;
    food_tile_ = nullptr;
    food_point_ = DEFAULT_POINT_VALUE;

    //Get the game parameters from the spinboxes
    width_ = ui->width_spin_box->value();
    height_ = ui->height_spin_box->value();
    seed_ = ui->seed_spin_box->value();

    //Create a new game
    gameboard_ = new GameBoard(width_,height_,seed_);

    //Create gui game board
    init_scene();

    //Start timers
    timer_.start(TICK_DURATION);
    clock_.start(SECOND_IN_MILLISECEONDS);

    //Update start button text
    ui->start_push_button->setText(PUSH_BUTTON_PAUSE_TEXT);
    //Update the GUI game timer
    update_gui_timer();
    //Update the GUI score
    ui->score_text_browser->setText(QString::number(current_score_));
    //Hide game info
    ui->game_info_text_browser->setVisible(false);

}

//If the game is over this function is called
//and it sets parameters and the GUI to match that state
//This is also the way that the user is informed of them winning or losing
void MainWindow::game_over() {

    //Set game status variables
    game_is_running_ = false;
    game_exists_ = false;
    //Stop the timers
    toggle_timer_run_status();

    //Check which end scenario the game reached
    //and update the GUI to match it
    if(gameboard_->game_won()){
       qDebug() << GAME_WON_DEBUG_MESSAGE;
       change_info_text(GAME_WON_MESSAGE);
       //Paint the walls of the GUI gamefield green
       change_wall_color(Qt::green);
    } else if(gameboard_->game_lost()){
       qDebug() << GAME_LOST_DEBUG_MESSAGE;
       change_info_text(GAME_LOST_MESSAGE);
       //Paint the walls of the GUI gamefield red
       change_wall_color(Qt::red);
     }

    //Update Start button and the info text visibility to match
    //the state of there not existing a active game
    ui->start_push_button->setText(PUSH_BUTTON_PLAY_TEXT);
    ui->game_info_text_browser->setVisible(true);

}



//Function is used to add the value of the eaten food
//to the current game score and this change is also made visible
//in the GUI.
void MainWindow::increase_score(){

   //Increase score
   current_score_ += FOOD_SCORE_VALUE;

   qDebug() << SCORE_INCREASE_DEBUG_MESSAGE;

   //Update the GUI to match the increase in score
   ui->score_text_browser->setText(QString::number(current_score_));
}

//Sets the QGraphicsRectItem related constants
//based on the GUI graphics_view size and gameboard dimensions
void MainWindow::set_rectangle_variables() {

    if(scene_!=nullptr){
        //First get and adjust the background tile sizes
        tile_size_ = ui->graphics_view->size();
        //Tile sizes also include the walls of the gameboard and therefore
        //the tile size is found by divingin the total height by them of
        //the gameboard dimensions as well as the wall size
        tile_size_.rheight() /= height_+2*GAMEBOARD_WALL_THICKNESS;
        tile_size_.rwidth() /= width_+2*GAMEBOARD_WALL_THICKNESS;

        //Snake body tile values
        snake_tile_width_ = tile_size_.rwidth()
                *snake_body_to_background_tile_ratio;
        snake_tile_height_ = tile_size_.rheight()
                *snake_body_to_background_tile_ratio;
        snake_body_tile_x_offset_ = (tile_size_.rwidth()
                                    -snake_tile_width_)/2;
        snake_body_tile_y_offset_ = (tile_size_.rheight()
                                    -snake_tile_height_)/2;

        //Snake head tile values
        snake_head_tile_height_ = snake_tile_height_
                *snake_head_to_body_tile_ratio;
        snake_head_tile_width_ = snake_tile_width_
                *snake_head_to_body_tile_ratio;
        snake_head_tile_x_offset_ = (tile_size_.rwidth()
                                    -snake_head_tile_width_)/2;
        snake_head_tile_y_offset_ = (tile_size_.rheight()
                                    -snake_head_tile_height_)/2;

        //Snake tail tile values
        snake_tail_tile_height_ = snake_tile_height_
                *snake_tail_to_body_tile_ratio;
        snake_tail_tile_width_ = snake_tile_width_
                *snake_tail_to_body_tile_ratio;
        snake_tail_tile_x_offset_ = (tile_size_.rwidth()
                                    -snake_tail_tile_width_)/2;
        snake_tail_tile_y_offset_ = (tile_size_.rheight()
                                    -snake_tail_tile_height_)/2;

        //Food tile values
        food_tile_width_ = tile_size_.rwidth()*food_to_background_tile_ratio;
        food_tile_height_ = tile_size_.rheight()*food_to_background_tile_ratio;
        food_tile_y_offset_ = (tile_size_.rwidth()-food_tile_width_)/2;
        food_tile_y_offset_ = (tile_size_.rheight()-food_tile_height_)/2;

    } else {
        qDebug() << SET_CONSTANTS_ERROR_MESSAGE;
    }
}

//This function empties the GUI related tile vectors and their pointers
//as well as the food_tile_ pointer
void MainWindow::clear_scene(){
    erase_tile_vector(&snake_tiles);
    erase_tile_vector(&wall_tiles);
    delete food_tile_;

}

//Function deletes all of the pointers within the vector and the items
//relate to the scene removes the pointers
//from the vector
void MainWindow::erase_tile_vector(std::vector<QGraphicsRectItem *> *vector){
    QGraphicsRectItem* item_to_be_removed = nullptr;
    while(not vector->empty()){
        item_to_be_removed = vector->back();
        vector->pop_back();
        scene_->removeItem(item_to_be_removed);
        delete item_to_be_removed;
    }
    vector->clear();
}


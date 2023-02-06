/* snake_gui
 *
 * Short instructions:
 * Press PLAY or RESET to start to create a new game which will also
 * start the game.
 * If game is running press PAUSE to pause the game and to play to unpause it.
 * Press QUIT to exit the game
 * Use WASD to control the snake when the game is playing.
 *
 * Description:
 *  This is a GUI implementation of the base  snake game. In the game the goal
 *  is to move around the gameboard with a snake without the snake hitting its
 *  body or walls with its head and to eat as many pieces of food as possible.
 *  If a piece of food is eaten, a new one will be added to a tile that the
 *  snake does not already occupy and the snake  grows in length by a tile.
 *  At the beginning of the game, the snake only occupies a single tile.
 *  As the snake moves its tail follows its head but if the snake its a food
 *  the tail of the snake is extended. This results in the snake growing over
 *  the duration of the game. The user has won the game if the snake
 *  fills the entire board which is when the food has no place to be set
 *  and therefore the snake cannot grow any larger.
 *      The user can select the game board size by inputting their preferred
 *  map dimensions into the spinboxes that reside next to the gameboard.
 *  The user can also set the seed for the game this way as well. However,
 *  the changes to these values will only be taken into consideration when
 *  the user creates a new game - The changes will not affect the current game.
 *      The user can create a game by pressing start whenever a game is not
 *  already taking place or by hitting the reset button even when a game
 *  alread exists. If a game already exists the play button will work as
 *  a pause/resume button for the game. The game will end if the user wins
 *  or loses by hitting either the body of the snake or a wall with its head
 *  and the game will be over and cannot be continued any further.
 *  If a new game is started after this or during the game by pressing reset
 *  the size of the snake is returned back to its original size as well as
 *  the timer and the score is reset.
 *      When a game is running (exists and not paused), the user can
 *  select the directions in which the snake will move by using the
 *  wasd-keys on their keyboards. The snake can only move into one of these
 *  directions at once and therefore only the last input that is not the
 *  opposite direction of the snake currently will be direction the snake
 *  will move in.
 *      When the snake eats a fruit, it grows larger and the new tail is
 *  darker than previously. This darkening will also affect the snakes body
 *  as well. Eating a fruit will also increase the points of the user which
 *  are displayed in the GUI's score-box (text browser) which is updated every
 *  time the user eats a fruit. The score will reset to zero when the user
 *  starts a new game. The ammount of points received for eating a fruit is
 *  constant and does not change over the game.
 *      When a game ends the walls in the GUI gameboard turn red and a
 *  text indicating this is overlayed over the gameboard. This also happens
 *  when the user wins the game however the walls turn green instead of red
 *  and the message that appears refers to the user winning the game instead of
 *  losing. Also, if the user pauses the game. Pause-buttons text changes to
 *  play and a text indicating that the game is paused is overlayed on top of
 *  the GUI's gameboard. These messages and the color of the gameboards walls
 *  is reset when the player starts a new game or in the case of pausing the
 *  pause message is hidden when the user presses the play button.
 *      The duration that the player has spend on the game in seconds when only
 *  considering the time the game was not paused nor over is displayed to
 *  the user in the GUIs box that has the tile time which is updated every
 *  second the game is active. The time spend on the game is here indicated in
 *  seconds and will be reset if the current game ends and a new game is
 *  created.
 *      The application can be closed with the quit-button that closes the
 *  window.
 *
 *
 *
 *
 *
 *
 *  EXTRA Features:
 *  Gameboard size
 *  Gradient snake coloring
 *  Edit background color on win/lose/reset
 *  Game clock -timer
 *  Pause button    Note: In the form of a multi-purpose play-button
 *  Score
 *
 *
 *  Technical details:
 *  I shall refer to QGraphicsRectItems by calling the tiles. Rect would've
 *  been better but it too is an already existing class so the name tile
 *  was chosen instead.
 *
 *  The mainwindows key pararmeters are the snake_tiles_ vector which contains
 *  pointers to all of the tiles that represent the snake. These tiles are
 *  removed and others are added in their stead when the size of the tile
 *  (that corresponds to snake body, tail or head) does not match the new body
 *  part of the snake that should exist there instead.
 *      The vector of snake tiles is also used to control the color scheme of
 *  the tiles because at every tick of the game the snake moves and therefore
 *  the colors of the tiles need to also be changed due to the colors
 *  representing, how far from its head the current tile is (as in how many
 *  snake tiles are inbetween). This is done by calling the function
 *  alter_snake_tiles while the new head has not already been added to
 *  the snake_tiles_ vector.
 *      The logic behind the actions in the GUI that happen on every tick
 *  of the game tick timer is mainly done in the update_snake() function
 *  which is where the adding of the score redrawing parts of the snake and
 *  game score addition happens.
 *      The size of the gameboard is designed to match the GUI
 *  QGraphicsViews size and all of the widths and positions are calculated
 *  from this. This means that the size of the QGraphicsView can be altered
 *  and the scene that has the gameboard will match its size. This also
 *  means that the gameboard can look a bit funny when the gameboard dimensions
 *  are set to highly differ from oneanother but it will function the same.
 *  These tile variables are calculated every time the user creates a new game
 *  in the set_rectangle_constants function which is called by the init_scene
 *  function that draws the background tiles and walls as well as calls the
 *  update_snake and draw food to render these at the beginning of a game.
 *      The fact that the tiles aren't hardcoded also make the x and y
 *  position calculation difficult to read but this is in my view better
 *  due to the robustness of the scenery. If you're trying figure out
 *  how some of the coordinates are calculated, it helps to scetch out a
 *  grid with dimensions that are variables and to do an example calculation
 *  by hand first do derive a formula for the calculation. Also to optimize
 *  the calculations there are several variables that exist in the private
 *  section of the MainWindow class header file so that most of the
 *  calculations do not form as a conveluted formulas or take as many
 *  calculations as they would if the variables were calculated in each run of
 *  the functions.
 *      Most of the private functions created to implement this class do not
 *  have any parameters other than ones that through the differing parameters
 *  will have (useful) inpact on the application. This means that the only
 *  function that accepts parameters is the function for setting the color
 *  of the wall tiles in the case of a victory or loss.
 *
 *      The snake game is controlled with keyboard inputs (wasd) which are read
 *  by the keyPressEvent function that will override the latestdirection
 *  with a new direction indicated by the key given that the key is not
 *  the opposite of the previously chosen direction. The direction is however
 *  only taken into consideration when the game tick is finally called by
 *  the timer_ (game tick clock) which means that if the user inputs first
 *  w and then s during a game tick when the previous direction was a that
 *  the latter direction is selected to be the next direciton of movement.
 *
 *      In the GUI there are only three buttons: start, reset and quit.
 *  Reset starts a new game and quit closes the GUI window. The start-button
 *  however has multiple purposes of being the key start is used to both
 *  start the game, pause the game and to unpause it. The logic is:
 *  (1) If there is no active game the button press will start a new game.
 *  (2) If game exists, pause or unpause it given the status of the game
 *  (paused, unpaused).
 *
 *      The game "engine" exists within the gameboard class which the
 *  mainwindow creates an instance of and usese that to run the
 *  snake game by using the movesnake command. The mainwindow uses
 *  the get_snake_head_point function of the gameboard to get the position of
 *  the snake, get_food_point to get the position of the food and
 *  uses this with the snake_length function (my own) to render the snake
 *  and to render the food (if the food was eaten checked by reading if
 *  the snake head moved to the food tile).
 *
 *      Default values as well as debug and all GUI text is stored into
 *  constants that are used as such. and are defined in the mainwindow
 *  header file before the class definition.
 *
 *
 * Program author
 * Name: Max Väistö
 * Student number: K440046
 * Username: rnmava
 * E-Mail: max.vaisto@tuni.fi
 *
 * About this program and its programming:
 *  I have done this solo.
 *  Some single lines of code have taken inspiration
 *  from external sources however every function was created by me
 *  and the logic behind them is also of my own creation.
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QTextBrowser>
#include <QTimer>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "gameboard.hh"
#include "GUI_Constants.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//Constatns are in GUI_Constants.hh

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //Sets up the GUI for the mainwindow as well as sets
    //the pallets for the brushes and pens used in the
    //QGraphicsScene
    MainWindow(QWidget *parent = nullptr);

    //Used to when the mainwindow is closed to remove
    //all of the pointer entities
    ~MainWindow();

    //Used to monitor snake movement related keypresses
    //If such input is received while the game is running
    //without it being the opposite direction of the last snake direction,
    //the next snake movement direction will be set as the input direction.
    void keyPressEvent(QKeyEvent* event) override;

private slots:

    //This is used to monitor the clicks regarding the start button
    //The start button has two modes:
    //(1) Game exists -> Pause / Play and toggle timers
    //(2) Game doesnt exist -> Create new game
    void on_start_push_button_clicked();

    //Called if the user presses the reset button which will create a new game
    void on_reset_push_button_clicked();

private:
    Ui::MainWindow *ui;
    GameBoard* gameboard_ = nullptr;    // Snake game gameboard
    QGraphicsScene* scene_;              // GUI gameboard scene
    QTimer timer_;                      // For snake speed
    QTimer clock_;                      // For game time
    int game_time_ = 0;                 // Game time value



    //GUI brushes and pens
    QBrush background_tile_odd_brush_ = QBrush();
    QBrush background_tile_even_brush_ = QBrush();
    QPen background_tile_odd_pen_ = QPen();
    QPen background_tile_even_pen_ = QPen();
    QBrush background_wall_brush_ = QBrush();
    QPen background_wall_pen = QPen();
    QBrush snake_brush_ = QBrush();
    QPen snake_pen_ = QPen();
    QBrush food_brush_ = QBrush();
    QPen food_pen_ = QPen();


    //Gameboard background tile size
    QSize tile_size_ = QSize(0,0);

    //Snake tile dimension size variables
    int snake_tile_width_ = 0;
    int snake_tile_height_ = 0;
    int snake_head_tile_height_ = 0;
    int snake_head_tile_width_ = 0;
    int snake_tail_tile_width_ = 0;
    int snake_tail_tile_height_ = 0;
    //Food tile dimension size variables
    int food_tile_width_ = 0;
    int food_tile_height_ = 0;

    //Tile offsets are used to place the
    //tiles in the middle of the background tiles
    int snake_body_tile_x_offset_ = 0;
    int snake_body_tile_y_offset_ = 0;
    int snake_head_tile_x_offset_ = 0;
    int snake_head_tile_y_offset_ = 0;
    int snake_tail_tile_x_offset_ = 0;
    int snake_tail_tile_y_offset_ = 0;
    int food_tile_x_offset_ = 0;
    int food_tile_y_offset_ = 0;



    //The directions the snake will next and has last moved in
    std::string next_direction_;
    std::string last_direction_ ;

    //Food GUI scene pointer and the default point of food
    QGraphicsRectItem* food_tile_ = nullptr;
    Point food_point_ = DEFAULT_POINT_VALUE;

    //Snake and wall tile vectors
    std::vector<QGraphicsRectItem*> snake_tiles;
    std::vector<QGraphicsRectItem*> wall_tiles;

    //This is used to check wether the game is running
    bool game_is_running_ = false;
    //This is used to check wether a game exists
    bool game_exists_ = false;
    //Initialize the current score
    int current_score_ = DEFAULT_GAME_SCORE;

    //Gameboard seed and dimension variables
    int seed_ = DEFAULT_SEED;
    unsigned int width_ = DEFAULT_WIDTH;
    unsigned int height_ = DEFAULT_HEIGTH;


    //Timers

    //Clock_tick function called by the game duration clock
    //which increases the game time by a second
    //and updates the GUI to match this change
    void clock_tick(); //Game duration timer

    //This is used to update the GUI game length clock
    void update_gui_timer(); //Game duration timer GUI update

    //Called by the game speed clock
    //Continues to move and to render the snake as long as the game is not over
    //and if the game is over the function will call the game_over function
    void update_game(); //Game tick

    //This is used to pause and to continute game clocks
    //There are two because one is for the game time count clock
    //and the other is for the game tick timer
    void toggle_timer_run_status();


    //  GUI graphics view rendering


    // Scene initialization

    //Function init_scene is used to draw the gameboard which
    //erases the old graphics_view scene and draws the gameboard onto it.
    //init_scene will call on the update_snake and draw_food functions
    //to draw these to the empty GUI gameboard.
    //This function also erases the previous gameboard and
    void init_scene();

    //      Snake tile addition and replacement

    //Adds snake parts which vary in location and size depending on the
    //type given in the parameters (isHead)
    //point pararmeter is used to receive the (rough) coordinates
    //of the new snake tile
    void add_snake_head();

    //This is used to change replace the head of the snake
    //with a snake body tile.
    //The function will
    // (1) Get the position of the head tile and calculate
    //  the position of the new tile with it
    // (2) Remove the old head tile
    // (3) Add new tile to the end of snake_tiles
    void replace_head_with_body();

    //This is used to change replace the last of the snake
    // with a snake tail tile.
    //The function will
    // (1) Get the position of the last tile and calculate
    //  the position of the new tile
    // (2) Replace and delete the old end tile with the new tail tile
    void replace_snake_end_with_tail();

    //This is used to remove the old food and to draw a new one
    //the function calls the get_food_point function to retreive the
    //location of the food point
    void draw_food();

    //Used to alter the snake tiles of the snake in color
    //to give the snake a color that differs on every snake tile
    void alter_snake_tile_colors();

    //Used to change the colors of the walls when the game is won or lost
    //The parameter color is the QColor that the brushes and pens of the
    //tiles will be set to.
    void change_wall_color(QColor color);

    //This function is used to set the text for the
    //gui element info_text_browser so that the text
    //is centered and it has a outline
    void change_info_text(QString new_text);


    //  Gameboard operations

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
    void update_snake();

    //create_new_game function is used to initialize a new game
    //The function will in order:
    // (1) Erase the old gameboard
    // (2) Reset game related variables
    // (3) Get gameboard dimensions and creates it and calls
    //  for it to be rendered.
    // (4) Starts game timers
    // (5) Hide items blocking the gameboard in the GUI
    void create_new_game();

    //If the game is over this function is called
    //and it sets parameters and the GUI to match that state
    //This is also the way that the user is informed of them winning or losing
    void game_over();


    //Score

    //Function is used to add the value of the eaten food
    //to the current game score and this change is also made visible
    //in the GUI.
    void increase_score();


    //Variable initialization

    //Sets the QGraphicsRectItem related constants
    //based on the GUI graphics_view size and gameboard dimensions
    void set_rectangle_variables();


    //Cleaning dead pointers

    //This function empties the GUI related tile vectors and their pointers
    //as well as the food_tile_ pointer
    void clear_scene();

    //Function deletes all of the pointers within the vector and the items
    //relate to the scene removes the pointers
    //from the vector
    void erase_tile_vector(std::vector<QGraphicsRectItem*>* vector);

};

#endif // MAINWINDOW_HH

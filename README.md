# Snake-GUI
Snake game GUI implementation in QT Creator and C++. This was done as part of the COMP.CS.110 Programming 2: Structures TUNI course. 
The task for this project was to implement the already written C++ ASCII snake game into a GUI version with QTCreator. 
There are also extra features that were added to the game such as snake body has differing colors, custom game board dimensions, ect.
full list of all the features and instruction for the game are in the mainwindow.hh file. 

This project got in total 97/100 points.

This code was written in the December of 2022. 

Description:
This is a GUI implementation of the base  snake game. In the game the goal
is to move around the gameboard with a snake without the snake hitting its
body or walls with its head and to eat as many pieces of food as possible.
If a piece of food is eaten, a new one will be added to a tile that the
snake does not already occupy and the snake  grows in length by a tile.
At the beginning of the game, the snake only occupies a single tile.
As the snake moves its tail follows its head but if the snake its a food
the tail of the snake is extended. This results in the snake growing over
the duration of the game. The user has won the game if the snake
fills the entire board which is when the food has no place to be set
and therefore the snake cannot grow any larger.
    
The user can select the game board size by inputting their preferred
map dimensions into the spinboxes that reside next to the gameboard.
The user can also set the seed for the game this way as well. However,
the changes to these values will only be taken into consideration when
the user creates a new game - The changes will not affect the current game.

The user can create a game by pressing start whenever a game is not
already taking place or by hitting the reset button even when a game
alread exists. If a game already exists the play button will work as
a pause/resume button for the game. The game will end if the user wins
or loses by hitting either the body of the snake or a wall with its head
and the game will be over and cannot be continued any further.
If a new game is started after this or during the game by pressing reset
the size of the snake is returned back to its original size as well as
the timer and the score is reset.

When a game is running (exists and not paused), the user can
select the directions in which the snake will move by using the
wasd-keys on their keyboards. The snake can only move into one of these
directions at once and therefore only the last input that is not the
opposite direction of the snake currently will be direction the snake
will move in.

When the snake eats a fruit, it grows larger and the new tail is
darker than previously. This darkening will also affect the snakes body
as well. Eating a fruit will also increase the points of the user which
are displayed in the GUI's score-box (text browser) which is updated every
time the user eats a fruit. The score will reset to zero when the user
starts a new game. The ammount of points received for eating a fruit is
constant and does not change over the game.

When a game ends the walls in the GUI gameboard turn red and a
text indicating this is overlayed over the gameboard. This also happens
when the user wins the game however the walls turn green instead of red
and the message that appears refers to the user winning the game instead of
losing. Also, if the user pauses the game. Pause-buttons text changes to
play and a text indicating that the game is paused is overlayed on top of
the GUI's gameboard. These messages and the color of the gameboards walls
is reset when the player starts a new game or in the case of pausing the
pause message is hidden when the user presses the play button.

The duration that the player has spend on the game in seconds when only
considering the time the game was not paused nor over is displayed to
the user in the GUIs box that has the tile time which is updated every
second the game is active. The time spend on the game is here indicated in
seconds and will be reset if the current game ends and a new game is
created.

The application can be closed with the quit-button that closes the
window.

# 2D-Invaderers

## Brief

A 2D invaderers game implemented with the octet framework. The framework is available 
in https://github.com/andy-thomason/octet

## Project location and structure

The project is located in src/examples/invaderers and it is based on the one that is
located in src/examples/example_invaderers. 

The folder agarzonp contains almost all the contribution done to this project. The folder
itself contains additional directories:

CSVParser: It contains a class to parse csv files
Gameplay: It contains classes that deal with all the gameplay logic
Input: It contains a class that wraps octet input handling
Sound: It contains classes that deals with the sound
Render: It contains a class to draw text

This project structure has been designed with the purpose to reuse as much as code as possible
when it comes to develop gameplay logic with octet. In part, thanks to the architecture implemented
too.

## Architecture

The example that this project is based on, contains all the game logic in one monoholitic
class. This clearly is not a good software design approach from a maintainability, flexibility and 
scalability point of view. Therefore, a new design has been implemented following some of the
design patterns found in http://gameprogrammingpatterns.com/ . The use of State, Singleton, Factory,
Type-Object and Object Pool patterns are the ones that has been used.

The main challenge when implementing this architecture has been to deal with all the circular dependencies
that arose because of octet´s one compilation unit approach. Because no cpp file was used, forward declare
the classes wasn´t enough. The use of interfaces to communicate with the dependencies fixed the problem.

- Octet app and Gameplay

The octet app (invaderers_app.h) has been completely modified to just keep an instance of GamePlay. 
This instance is the one responsible to update/render all the game. In theory, we could attach a GamePlay
object to any app and with minor tweaks under GamePlay folder create different games using the same architecture.

- GameStateMachine and GameState

The overall gameplay code is ruled by a GameStateMachine that handles stackable GameStates. The GameStateMachine
is responsible for creating the game states, updating the top most state and rendering all the states. Each state must
be registered in GameStatesIds.h with an id. The game state id is the one that is used to create and transition to that
specific state.

Transitions between the states is handle by different states according to input, gameplay logic, etc.

Each specific state must inherits from GameState. For this project, three are the states that has been implemented: 

BattleState: It sets the world, load the level, etc.
PauseState: A dummy state that pauses the game. The transition to/from this state when pressing key_esc
DebugMenuState: An specific state to enter debug/cheat commands in the console window. The transition to this
state is by pressing the key_f1.

Additionally, each game state has the ability to receive any initialization parameter if needed.

- GameObjects

Any entity that needs some kind of gameplay logic or collisions must need to inherit from this class and override
the corresponding methods. For example, we have Player, Missile, Invaderer and much more classes that inherits from
GameOject and override specific methods to implement their own behaviours.

Each GameObject has an octet sprite that uses to render the sprite attached to the object.

- GameObjectPool, GameObjectFactory and World

GameObjectPool keeps different pools for each game object in the game, so there is no need to create game objects dynamically.
Gameplay logic is responsible to set the inUse variable of each game object to make sure that an object is active or not. The pool
only gives non active game objects.

GameObjectFactory is the one  who knows how to really create and initialise the specific game objects. It uses an interface to
get game objects from the pool, when a new game object is created.

The World is the class responsible to load the level and create the different game objects by using the factory. Any time a new
game object is requested, it must be added to the World to make sure that is going to be updated/rendered.

- Data-driven

CSV file format parser is used to read any game configuration. 

For this project, it is used to load the level and create the different invaderers. An example of a level is shown below:

Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_2,
Invader_1,Invader_1,Invader_1,Invader_1,Invader_3,,,Invader_1,,
Invader_2,,Invader_1,Invader_1,,,,Invader_1,Invader_1,
,,Invader_1,Invader_1,,,,Invader_1,Invader_1,
Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1
,Invader_1,,Invader_1,Invader_1,,Invader_1,Invader_1,,

- Sounds

Any sound to be used in the game, must be registered in SoundIds.h
The id given to the sound is the one that is used to play any sound from anywhere in the code by the SoundManager







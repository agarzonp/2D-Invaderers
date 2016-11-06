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

The octet app (invaderers_app.h) has been completely modified to just keep an instance of Gameplay. 
This instance is the one responsible to update/render all the game. In theory, we could attach a Gameplay
object to any app and with minor tweaks insdie the Gameplay folder create different games using the same architecture.

- GameStateMachine and GameState

The overall gameplay code is ruled by a GameStateMachine that handles stackable GameStates. The GameStateMachine
is responsible for creating the game states, updating the top most state and rendering all the states. Each state must
be registered in GameStatesIds.h with an id. The game state id is the one that is used to create and transition to that
specific GameState.

Transitions between the states is handle by different states according to input, gameplay logic, etc.

Each specific state must inherits from GameState. For this project, three are the states that has been implemented: 

BattleState: It sets the world, load the level, etc.
PauseState: A dummy state that pauses the game. The transition to/from this state happens when pressing key_esc
DebugMenuState: An specific state to enter debug/cheat commands in the console window. The transition to this
state is by pressing the key_f1.

Additionally, each game state has the ability to receive any initialization parameter if needed.

- GameObjects

Any entity that needs some kind of gameplay logic or collisions must need to inherit from this class and override
the corresponding methods. For example, we have Player, Missile, Invaderer and much more classes that inherits from
GameObject and override specific methods to implement their own behaviours.

Each GameObject has an octet sprite that uses to render the sprite attached to the object.

- GameObjectPool, GameObjectFactory and World

GameObjectPool keeps different pools for each game object in the game, so there is no need to create game objects dynamically.
Gameplay logic is responsible to set the inUse variable of each game object to make sure that a GameObject is active or not. The pool
only gives non active game objects.

GameObjectFactory is the one  who knows how to really create and set the specific game objects. It uses an interface to
get a GameObject from the pool when a new game object is requestd to be created.

The World is the class responsible to load the level and create the different game objects by using GameObjectFactory. Any time a new
game object is requested, it must be added to the World to make sure that is going to be updated/rendered.

- Data-driven

A CSV format file parser is used to read any game configuration. 

For this project, it is used to load the level and create the different types invaderers. 

- Sounds

Any sound to be used in the game, must be registered in SoundIds.h
The id given to the sound is the one that is used to play any sound from anywhere in the code by the SoundManager.

## Gameplay

The game consist about killing all the invaderers for each level in order to progress to the next level. Those invaderers belong to an InvadererWave game object that is the one who keep tracks of the invaderers. The InvadererWave is responsible to move the invaderers and decide who is the invaderer that is going to shoot. 

The invaderers are positioned in the world in a grid layout manner. In a CSV file, it is defined where to position each invaderer. Additionaly, the type of invaderer and the size that it occupies in the grid is defined. An example of a wave set up is shown below:

Invader_1,Invader_1,InvaderHard_2,,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1
Invader_1,Invader_1,,,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1
Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1
Invader_1,Invader_1,Invader_1,Invader_1,InvaderHard_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_1
Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,Invader_2,,Invader_1,Invader_1,Invader_1
Invader_1,Invader_1,Invader_1,Invader_1,Invader_1,,,Invader_1,Invader_1,Invader_1

In the example above, we are positioning an invaderer of type InvaderHard and size 2 at position [0][2]. 

There are different types of invaderers(Invader and InvaderHard). The type of the invaderer and the size of that invaderer, will define different properties. Those properties are basic things like lives of the invaderer, the score you get when killing one of those invaderers and the texture to use.








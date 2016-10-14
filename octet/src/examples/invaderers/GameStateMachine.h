#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include <vector>

enum GameStateId
{
	GAME_STATE_INIT,

	GAME_STATE_MENU,
	GAME_STATE_BATTLE,
	GAME_STATE_PAUSE,
	GAME_STATE_RANKING,

	GAME_STATE_EXIT,

	NUM_GAME_STATES
};

class GameState;

class GameStateMachine
{
public:
	static GameStateMachine* GetInstance();
	~GameStateMachine();

	void SetState(GameStateId gameStateId);
	void PushState(GameStateId gameStateId);
	void PopState();

	void UpdateState();
	void RenderState();

protected:
	GameStateMachine();

private:

	void Clear();
	GameState* CreateState(GameStateId gameStateId);

	std::vector<GameState*> states;
	//octet::containers::dynarray<GameState*> states;

	static GameStateMachine* s_instance;
};


#endif




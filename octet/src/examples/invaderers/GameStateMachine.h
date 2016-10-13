#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H


#include <vector>

namespace octet
{
	enum GameStateId
	{
		GAME_STATE_INIT,
		GAME_STATE_MENU,
		GAME_STATE_BATTLE,
		GAME_STATE_RANKING,

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

		std::vector<GameState*> states;

		static GameStateMachine* s_instance;
	};
}

#endif




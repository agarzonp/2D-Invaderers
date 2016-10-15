#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "GameStatesDefs.h"

namespace agarzonp
{
	class GameStateMachine
	{
		octet::containers::dynarray<GameState*> states;

		static GameStateMachine* s_instance;

	public:

		// singleton pattern. There could be problems in a multithreaded environment?
		static GameStateMachine* GetInstance()
		{
			if (!s_instance)
			{
				s_instance = new GameStateMachine();
			}

			return s_instance;
		}

		~GameStateMachine()
		{
			Clear();
			s_instance = nullptr;
		}

		void SetState(GameStateId gameStateId)
		{
			Clear();

			states.push_back(CreateState(gameStateId));

			states.back()->Start();
		}

		void PushState(GameStateId gameStateId)
		{
			if (states.size() > 0)
			{
				states.back()->Suspend();
			}

			states.push_back(CreateState(gameStateId));

			states.back()->Start();
		}

		void PopState()
		{
			if (states.size() > 0)
			{
				states.back()->Stop();

				delete states.back();
				states.pop_back();

				states.back()->Resume();
			}
		}

		void UpdateState()
		{
			if (states.size() > 0)
			{
				states.back()->Update();
			}
		}

		void RenderState()
		{
			for (auto state : states)
			{
				state->Render();
			}
		}

	protected:
		GameStateMachine()
		{
			// it might assert in a multithreaded environment. Fix the singleton!
			assert(!s_instance);
		}

	private:

		void Clear()
		{
			if (states.size() > 0)
			{
				for (auto state : states)
				{
					state->Stop();
					delete state;
				}
				states.resize(0);
			}
		}

		GameState* CreateState(GameStateId gameStateId)
		{
			GameState* state = nullptr;

			switch (gameStateId)
			{
			case GameStateId::BATTLE:
				state = new BattleState();
				break;
			case GameStateId::PAUSE:
				state = new PauseState();
				break;
			default:
				assert(false); // GameStateId not handled!
				state = new GameState();
				break;
			}

			return state;
		}
	};

	GameStateMachine* GameStateMachine::s_instance = nullptr;
}


#endif




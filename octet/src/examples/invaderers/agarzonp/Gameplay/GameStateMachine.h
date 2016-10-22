#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "GameStatesIds.h"
#include "GameStateParams.h"
#include "GameStateMachineInterface.h"
#include "GameStates.h"

namespace agarzonp
{
	class GameStateMachine final : public GameStateMachineInterface
	{
		octet::containers::dynarray<GameState*> states;

	public:

		GameStateMachine() {}
		~GameStateMachine() {Clear();}

		void SetState(GameStateId gameStateId, GameStateParams* params = nullptr) final
		{
			Clear();

			states.push_back(CreateState(gameStateId));

			states.back()->Start(params);
		}

		void PushState(GameStateId gameStateId, GameStateParams* params = nullptr) final
		{
			if (states.size() > 0)
			{
				states.back()->Suspend();
			}

			states.push_back(CreateState(gameStateId));

			states.back()->Start(params);
		}

		void PopState() final
		{
			assert(states.size() > 1);
			if (states.size() > 0)
			{
				states.back()->Stop();

				delete states.back();
				states.pop_back();

				if (states.size() > 0)
				{
					states.back()->Resume();
				}
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
				state = new BattleState(this);
				break;
			case GameStateId::DEBUG_MENU:
				state = new DebugMenuState(this);
				break;
			case GameStateId::PAUSE:
				state = new PauseState(this);
				break;
			default:
				assert(false); // GameStateId not handled!
				state = new GameState(this);
				break;
			}

			return state;
		}
	};
}

#endif




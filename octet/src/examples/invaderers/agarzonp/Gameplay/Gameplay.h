#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "GameStateMachine.h"	

namespace agarzonp
{
	class Gameplay
	{
		GameStateMachine gameStateMachine;

	public:

		Gameplay() {}
		~Gameplay() { Finish(); }

		void Init(const octet::app& app)
		{
			// Needed for getting the input across different game states
			agarzonp::Input::SetInput(app);

			BattleStateParams* params = new BattleStateParams();
			params->level = 0;

			gameStateMachine.SetState(GameStateId::BATTLE, params);
		}

		void Update() 
		{
			gameStateMachine.UpdateState();	

			if (Input::is_key_going_down(octet::key_f1))
			{
				gameStateMachine.PushState(GameStateId::DEBUG_MENU);
				return;
			}
		}

		void Render()
		{
			gameStateMachine.RenderState();	
		}

	private:

		void Finish() {}
	};
}


#endif // !


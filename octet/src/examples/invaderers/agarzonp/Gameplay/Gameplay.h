#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "GameStateMachine.h"	

namespace agarzonp
{
	class Gameplay
	{
		GameStateMachine* gameStateMachine;

	public:

		Gameplay() : gameStateMachine(nullptr) {}
		~Gameplay() { Finish(); }


		void Init(const octet::app& app)
		{
			// Needed for getting the input across different game states
			agarzonp::Input::SetInput(app);

			assert(!gameStateMachine);

			gameStateMachine = GameStateMachine::GetInstance();
			gameStateMachine->SetState(GameStateId::BATTLE);
		}

		void Finish()
		{
			if (gameStateMachine)
			{
				delete gameStateMachine;
			}
		}

		void Update() 
		{
			if (gameStateMachine)
			{
				gameStateMachine->UpdateState();
			}
		}

		void Render()
		{
			if (gameStateMachine)
			{
				gameStateMachine->RenderState();
			}
		}
	};
}


#endif // !


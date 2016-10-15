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

			gameStateMachine.SetState(GameStateId::BATTLE);
		}

		void Update() 
		{
			gameStateMachine.UpdateState();	
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


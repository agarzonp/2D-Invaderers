#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "../Sound/SoundManager.h"
#include "../Render/DrawText.h"
#include "GameStateMachine.h"	

namespace agarzonp
{
	class Gameplay
	{
		SoundManager* soundManager;
		TextDrawer* textDrawer;

		GameStateMachine gameStateMachine;

	public:

		Gameplay() 
			: soundManager (nullptr)
			, textDrawer(nullptr)
		{
		}

		~Gameplay() { Finish(); }

		void Init(const octet::app& app)
		{
			// Needed for getting the input across different game states
			agarzonp::Input::SetInput(app);

			// SoundManager
			soundManager = SoundManager::GetInstance();
				
			// TextDrawer
			textDrawer = TextDrawer::GetInstance();

			// Set the first state
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

		void Finish() 
		{
			delete soundManager;
			delete textDrawer;
		}
	};
}


#endif // !


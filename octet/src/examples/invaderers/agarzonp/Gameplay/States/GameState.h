#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace agarzonp
{
	class GameState
	{
	protected:
		// The interface used by the state to communicate with the game state machine
		GameStateMachineInterface* gameStateMachineInterface;

		// parameters for setting the state. The state is the one responsible for deleting the parameters
		GameStateParams* stateParams;
	public:
		GameState(GameStateMachineInterface* gsmInterface) 
			: gameStateMachineInterface(gsmInterface)
			, stateParams(nullptr)
		{
		};

		virtual ~GameState() 
		{
			if (stateParams)
			{
				delete stateParams;
			}
		};

		virtual void Start(GameStateParams* params = nullptr)
		{ 
			stateParams = params; 
		}

		virtual void Stop() {}
		virtual void Suspend() {}
		virtual void Resume() {}
		virtual void Update() {}
		virtual void Render() {}
	};
}

#endif


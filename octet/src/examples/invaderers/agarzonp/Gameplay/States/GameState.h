#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace agarzonp
{
	class GameState
	{
	protected:
		// The interface used by the state to communicate with the game state machine
		GameStateMachineInterface* gameStateMachineInterface;
	public:
		GameState(GameStateMachineInterface* gsmInterface) : gameStateMachineInterface(gsmInterface){};
		virtual ~GameState() {};

		virtual void Start() {};
		virtual void Stop() {};
		virtual void Suspend() {};
		virtual void Resume() {};
		virtual void Update() {};
		virtual void Render() {};
	};
}

#endif


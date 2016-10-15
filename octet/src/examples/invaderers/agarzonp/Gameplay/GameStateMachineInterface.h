#ifndef GAME_STATE_MACHINE_INTERFACE_H
#define GAME_STATE_MACHINE_INTERFACE_H

namespace agarzonp
{
	class GameStateMachineInterface
	{
	public:

		virtual~GameStateMachineInterface(){}

		virtual void SetState(GameStateId gameStateId) = 0;
		virtual void PushState(GameStateId gameStateId) = 0;
		virtual void PopState() = 0;
	};
}

#endif // !GAME_STATE_MACHINE_INTERFACE_H


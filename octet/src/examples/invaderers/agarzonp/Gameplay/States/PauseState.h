#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

namespace agarzonp
{
	class PauseState : public GameState
	{
	public:
		PauseState(GameStateMachineInterface* gsmInterface)
			: GameState(gsmInterface)
		{
		}

		~PauseState() {};

		void Start() override {};
		void Stop() override {};
		void Suspend() override {};
		void Resume() override {};
		void Update() override 
		{
			if (Input::is_key_going_down(octet::key_f1))
			{
				gameStateMachineInterface->PopState();
			}
		}

		void Render() override {};
	};
}

#endif // !PAUSE_STATE_H





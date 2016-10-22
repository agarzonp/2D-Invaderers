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

		~PauseState() {}

		void Start(GameStateParams* params) override
		{
			GameState::Start(params);
		}

		void Stop() override 
		{
			GameState::Stop();
		}

		void Suspend() override 
		{
			GameState::Suspend();
		}

		void Resume() override 
		{
			GameState::Resume();
		}

		void Update() override 
		{
			GameState::Update();

			if (Input::is_key_going_down(octet::key_esc))
			{
				gameStateMachineInterface->PopState();
			}
		}

		void Render() override 
		{
			GameState::Render();
		}
	};
}

#endif // !PAUSE_STATE_H





#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "GameState.h"

class PauseState : public GameState
{
public:
	PauseState();
	~PauseState();

	void Start() override;
	void Stop() override;
	void Suspend() override;
	void Resume() override;
	void Update() override;
	void Render() override;
};

#endif // !PAUSE_STATE_H





#ifndef GAME_STATE_H
#define GAME_STATE_H

class GameState
{
public:
	GameState();
	virtual ~GameState();

	virtual void Start();
	virtual void Stop();
	virtual void Suspend();
	virtual void Resume();
	virtual void Update();
	virtual void Render();

protected:
		
};

#endif


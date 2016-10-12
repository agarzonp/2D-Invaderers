#ifndef GAME_STATE_H
#define GAME_STATE_H

namespace octet
{
	class GameState
	{
	public:
		GameState();
		~GameState();

		void Start();
		void Stop();

		void Update();
		void Render();
	};
}

#endif


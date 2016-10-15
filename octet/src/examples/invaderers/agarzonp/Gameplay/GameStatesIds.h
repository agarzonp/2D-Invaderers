#ifndef GAME_STATES_IDS_H
#define GAME_STATES_IDS_H

namespace agarzonp
{
	enum class GameStateId : uint8_t
	{
		INIT,

		MENU,
		BATTLE,
		PAUSE,
		RANKING,

		EXIT
	};
}

#endif // !GAME_STATES_IDS_H


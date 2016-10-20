#ifndef GAME_STATES_IDS_H
#define GAME_STATES_IDS_H

namespace agarzonp
{
	enum class GameStateId : uint8_t
	{
		INIT,

		MENU,
		BATTLE,
		DEBUG_MENU,
		PAUSE,
		RANKING,

		EXIT,

		NUM_GAME_STATE_IDS
	};

	static const char* s_gameStateIdNames[] =
	{
		"init",
		"menu",
		"battle",
		"debugmenu",
		"pause",
		"ranking",
		"exit"
	};
}

#endif // !GAME_STATES_IDS_H


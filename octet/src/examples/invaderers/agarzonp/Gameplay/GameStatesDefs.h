#ifndef GAME_STATES_DEFS_H
#define GAME_STATES_DEFS_H

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

#include "States/GameState.h"

// All the specific states
//#include "States/InitState.h"
//#include "States/MenuState.h"
#include "States/BattleState.h"
#include "States/PauseState.h"	
//#include "States/RankingState.h"
//#include "States/ExitState.h"

#endif // !GAME_STATES_DEFS_H

